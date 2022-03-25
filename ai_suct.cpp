#include <chrono>
#include <cmath>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

#include "ai.hpp"

#include <iostream>

namespace AI {

    static std::mt19937 rng(0);

    using RewardMap = std::unordered_map<std::string, float>;

    struct State {
        Simulator::Board board;
        std::vector<std::string> turnOrder;
        std::vector<Simulator::Direction> selectedMoves;

        friend bool operator==(const State& t_s1, const State& t_s2);
    };

    struct StateHash {
        size_t operator()(const State& t_state) const noexcept;
    };

    struct Node {
        unsigned int visitCount;
        RewardMap rewards;
    };

    using NodeMap = std::unordered_map<State, Node, StateHash>;

    State suct_from_board(const Simulator::Board& t_board, const std::string& t_playerId);
    State suct_update_state(const State& t_state, Simulator::Direction t_move);
    void suct_update_node(const State& t_state, NodeMap& t_nodes, const RewardMap& t_rewards);

    RewardMap suct_evaluate_state(const State& t_state);
    RewardMap suct_mcts_rollout(const State& t_state);

    std::vector<Simulator::Direction> suct_get_unselected_moves(const State& t_state, const NodeMap& t_nodes);

    float suct_ucb(float t_reward, unsigned int t_n, unsigned int t_N, float t_c);
    Simulator::Direction suct_select_move(const State& t_state, const NodeMap& t_nodes, MCTSParameters t_params);

    RewardMap suct_mcts_iter(const State& t_state, NodeMap& t_nodes, MCTSParameters t_params);


    Simulator::Direction mcts_suct_player(const Simulator::Board& t_board, const std::string& t_playerId, MCTSParameters t_params) {
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;
        
        const auto t1 = high_resolution_clock::now();
        
        const State state = suct_from_board(t_board, t_playerId);
        
        NodeMap nodes;
        nodes[state] = Node{0, {}};

        while (duration_cast<milliseconds>(high_resolution_clock::now() - t1).count() < t_params.computeTime) {
            suct_mcts_iter(state, nodes, t_params);
        }

        const std::vector<Simulator::Direction> safeMoves = get_safe_moves(t_board, t_playerId);
        if (safeMoves.empty()) {
            return Simulator::Direction::UP;
        }

        Simulator::Direction bestMove = safeMoves[0];
        float bestMoveScore = -std::numeric_limits<float>::infinity();
        for (Simulator::Direction move : safeMoves) {
            const State nextState = suct_update_state(state, move);

            const float totalReward = nodes[nextState].rewards[t_playerId];
            const unsigned int visitCount = nodes[nextState].visitCount;

            if (visitCount != 0) {
                const float score = totalReward / static_cast<float>(visitCount);
                if (score > bestMoveScore) {
                    bestMove = move;
                    bestMoveScore = score;
                }
            }
        }

        // std::cout << "Nodes visited: " << nodes.size() << '\n';

        return bestMove;
    }

    RewardMap suct_mcts_iter(const State& t_state, NodeMap& t_nodes, MCTSParameters t_params) {
        if (t_state.board.is_game_over()) {
            return suct_evaluate_state(t_state);
        }
        else {
            const std::vector<Simulator::Direction> unselectedMoves = suct_get_unselected_moves(t_state, t_nodes);

            if (t_nodes.count(t_state) && !unselectedMoves.empty()) {
                const Simulator::Direction move = unselectedMoves[rng() % unselectedMoves.size()];

                const State newState = suct_update_state(t_state, move);

                RewardMap rewards = suct_mcts_rollout(newState);
                t_nodes[newState].rewards = rewards;
                t_nodes[newState].visitCount++;

                suct_update_node(t_state, t_nodes, rewards);

                return rewards;
            }
            else {
                const Simulator::Direction move = suct_select_move(t_state, t_nodes, t_params);
                const State newState = suct_update_state(t_state, move);

                RewardMap rewards = suct_mcts_iter(newState, t_nodes, t_params);
                suct_update_node(t_state, t_nodes, rewards);

                return rewards;
            }
        }
    }


    State suct_from_board(const Simulator::Board& t_board, const std::string& t_playerId) {
        const auto& snakes = t_board.get_snakes();
                
        std::vector<std::string> turnOrder;
        turnOrder.reserve(snakes.size());
        
        // Make SUCT player move first to promote defensive play
        turnOrder.push_back(t_playerId);
                
        for (const auto& [id, snake] : snakes) {
            if (id != t_playerId) {
                turnOrder.push_back(id);
            }
        }
        
        // Disable food spawning in search to reduce the number of nodes to be visited
        Simulator::Ruleset ruleset = t_board.get_ruleset();
        ruleset.spawnFood = false;
        return State{Simulator::Board{t_board, ruleset}, turnOrder, {}};
    }
    
    State suct_update_state(const State& t_state, Simulator::Direction t_move) {
        std::vector<Simulator::Direction> selectedMovesNew = t_state.selectedMoves;
        selectedMovesNew.push_back(t_move);
        
        if (selectedMovesNew.size() == t_state.turnOrder.size()) {
            std::unordered_map<std::string, Simulator::Direction> moves;
            for (unsigned int i = 0; i < t_state.turnOrder.size(); i++) {
                moves[t_state.turnOrder[i]] = selectedMovesNew[i];
            }
            Simulator::Board boardNew = t_state.board;
            boardNew.update(moves);
            
            return State{boardNew, t_state.turnOrder, {}};
        }
        
        return State{t_state.board, t_state.turnOrder, selectedMovesNew};
    }

    void suct_update_node(const State& t_state, NodeMap& t_nodes, const RewardMap& t_rewards) {
        for (const auto& [id, r] : t_rewards) {
            t_nodes[t_state].rewards[id] += r;
        }
        t_nodes[t_state].visitCount++;
    }

    std::vector<Simulator::Direction> suct_get_unselected_moves(const State& t_state, const NodeMap& t_nodes) {
        std::vector<Simulator::Direction> possibleMoves = 
            get_safe_moves(t_state.board, t_state.turnOrder[t_state.selectedMoves.size()]);
        
        auto eraseIt = std::remove_if(
            possibleMoves.begin(),
            possibleMoves.end(),
            [&t_state, &t_nodes](Simulator::Direction move) -> bool {
                const State nextState = suct_update_state(t_state, move);
                return t_nodes.count(nextState);
            }
        );
        possibleMoves.erase(eraseIt, possibleMoves.end());

        return possibleMoves;
    }

    RewardMap suct_evaluate_state(const State& t_state) {
        RewardMap result;
        for (const std::string& id : t_state.turnOrder) {
            result[id] = 0.0f;
        }
        
        const std::string* winner = t_state.board.get_winner();
        if (winner != nullptr) {
            result[*winner] = 1.0f;
        }

        return result;
    }

    RewardMap suct_mcts_rollout(const State& t_state) {
        static constexpr std::array<Simulator::Direction (*)(const Simulator::Board&, const std::string&), 2> STRATEGIES {
            avoid_walls_player,
            seek_food_player
        };

        State currentState = t_state;
        while(!currentState.board.is_game_over()) {
            const std::string& currentPlayerId = currentState.turnOrder[currentState.selectedMoves.size()];
            const auto strategy = STRATEGIES[rng() % STRATEGIES.size()];
            const Simulator::Direction move = strategy(t_state.board, currentPlayerId);
            currentState = suct_update_state(currentState, move);
        }
        return suct_evaluate_state(currentState);
    }

    float suct_ucb(float t_reward, unsigned int t_n, unsigned int t_N, float t_c) {
        if (t_n == 0) {
            return std::numeric_limits<float>::infinity();
        }

        return (t_reward / t_n) + t_c * std::sqrt(std::log(t_N) / t_n);
    }

    Simulator::Direction suct_select_move(const State& t_state, const NodeMap& t_nodes, MCTSParameters t_params) {
        const std::string& currentPlayerId = t_state.turnOrder[t_state.selectedMoves.size()];

        const std::vector<Simulator::Direction> safeMoves = get_safe_moves(t_state.board, currentPlayerId);
        if (safeMoves.empty()) {
            return Simulator::Direction::UP;
        }
        
        Simulator::Direction bestMove = safeMoves[0];
        float bestMoveUCB = -std::numeric_limits<float>::infinity();
        for (const Simulator::Direction move : safeMoves) {
            const State nextState = suct_update_state(t_state, move);

            auto nodeIt = t_nodes.find(nextState);

            // If we haven't visited this node then UCB value will be +inf
            if (nodeIt == t_nodes.end()) {
                bestMove = move;
                break;
            }
            
            const RewardMap& rewards = nodeIt->second.rewards;
            const auto rewardIt = rewards.find(currentPlayerId);
            const auto parentNodeIt = t_nodes.find(t_state);
            if (rewardIt != rewards.end() && parentNodeIt != t_nodes.end()) {
                const float r = rewardIt->second;
                const unsigned int n = nodeIt->second.visitCount;
                const unsigned int N = parentNodeIt->second.visitCount;

                const float ucb = suct_ucb(r, n, N, t_params.ucbConstant);
                if (ucb > bestMoveUCB) {
                    bestMove = move;
                    bestMoveUCB = ucb;
                }
            }
        }

        return bestMove;
    }

    bool operator==(const State& t_s1, const State& t_s2) {
        return
            (t_s1.turnOrder == t_s2.turnOrder) &&
            (t_s1.selectedMoves == t_s2.selectedMoves) &&
            (t_s1.board == t_s2.board);
    }

    size_t StateHash::operator()(const State& t_state) const noexcept {
        size_t result = Simulator::BoardHash{}(t_state.board);

        for (Simulator::Direction move : t_state.selectedMoves) {
            result = (result ^ (static_cast<size_t>(move) << 1)) >> 1;
        }

        return result;
    }

}

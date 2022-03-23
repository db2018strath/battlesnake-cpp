#include <chrono>
#include <cmath>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

#include "ai.hpp"

namespace AI {

    static std::mt19937 rng(0);

    using RewardMap = std::unordered_map<std::string, float>;

    struct State {
        const Simulator::Board board;
        const std::vector<std::string> turnOrder;
        const std::vector<Simulator::Direction> selectedMoves;
    };

    struct Node {
        unsigned int visitCount;
        RewardMap rewards;
    };

    State suct_from_board(const Simulator::Board& t_board, const std::string& t_playerId);
    State suct_update_state(const State& t_state, Simulator::Direction t_move);
    void suct_update_node(const State& t_state, const std::unordered_map<State, Node>& t_nodes, const RewardMap& t_rewards);

    RewardMap suct_evaluate_state(const State& t_state);
    RewardMap suct_mcts_rollout(const State& t_state);

    std::vector<Simulator::Direction> suct_get_unselected_moves(const State& t_state, const std::unordered_map<State, Node>& t_nodes);

    float suct_ucb(float t_reward, unsigned int t_n, unsigned int t_N);
    Simulator::Direction suct_select_move(const State& t_state, const std::unordered_map<State, Node>& t_nodes);

    RewardMap mcts_suct_iter(State t_state, std::unordered_map<State, Node>& t_nodes);
    

    Simulator::Direction mcts_suct_player(const Simulator::Board& t_board, const std::string& t_playerId, unsigned int t_time) {
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;
        
        const auto t1 = high_resolution_clock::now();
        
        const State state = suct_from_board(t_board, t_playerId);
        
        std::unordered_map<State, Node> nodes;
        nodes[state] = Node{0, {}};

        while (duration_cast<milliseconds>(high_resolution_clock::now() - t1).count() < t_time) {
            mcts_suct_iter(nodes, state);
        }

        Simulator::Direction bestMove = DIRECTIONS_MAP[0];
        float bestMoveReward = std::numeric_limits<float>::min();
        for (Simulator::Direction move : DIRECTIONS_MAP) {
            const State nextState = suct_update_state(state, move);
            
            const float reward = nodes[nextState].rewards[t_playerId];
            if (reward > bestMoveReward) {
                bestMove = move;
                bestMoveReward = reward;
            }
        }

        return bestMove;
    }

    RewardMap mcts_suct_iter(State t_state, std::unordered_map<State, Node>& t_nodes) {
        const auto winner = t_state.board.get_winner();
        if (winner.has_value()) {
            return suct_evaluate_state(t_state);
        }
        else {
            const std::vector<Simulator::Direction> unselectedMoves = suct_get_unselected_moves(t_state, t_nodes);

            if (t_nodes.count(t_state) && !unselectedMoves.empty()) {
                const Simulator::Direction move = unselectedMoves[rng() % unselectedMoves.size()];

                const State newState = suct_update_state(t_state, move);

                const RewardMap rewards = suct_mcts_rollout(newState);

                t_nodes[newState].rewards = rewards;
                t_nodes[newState].visitCount++;

                suct_update_node(t_state, t_nodes, rewards);

                return rewards;
            }
            else {
                const Simulator::Direction move = suct_select_move(t_state, t_nodes);
                const State newState = suct_update_state(t_state, move);

                const RewardMap rewards = mcts_suct_iter(newState, t_nodes);
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

    void suct_update_node(const State& t_state, std::unordered_map<State, Node>& t_nodes, const RewardMap& t_rewards) {
        for (const auto& [id, r] : t_rewards) {
            t_nodes[t_state].rewards[id] += r;
        }
        t_nodes[t_state].visitCount++;
    }

    std::vector<Simulator::Direction> suct_get_unselected_moves(const State& t_state, const std::unordered_map<State, Node>& t_nodes) {
        std::vector<Simulator::Direction> possibleMoves = 
            get_safe_moves(t_state.board, t_state.turnOrder[t_state.selectedMoves.size()]);
        
        auto eraseIt = std::remove_if(
            possibleMoves.begin(),
            possibleMoves.end(),
            [&t_state, &t_nodes](Simulator::Direction move) -> bool {
                const State nextState = suct_update_state(t_state, move);
                return !t_nodes.count(nextState);
            }
        );
        possibleMoves.erase(eraseIt, possibleMoves.end());
    }

    RewardMap suct_evaluate_state(const State& t_state) {
        RewardMap result;
        for (const std::string& id : t_state.turnOrder) {
            result[id] = 0.0f;
        }
        
        const auto winner = t_state.board.get_winner();
        if (winner.has_value()) {
            result[winner.value()] = 1.0f;
        }

        return result;
    }

    RewardMap suct_mcts_rollout(const State& t_state) {
        State currentState = t_state;
        while(!currentState.board.get_winner().has_value()) {
            const std::string& currentPlayerId = currentState.turnOrder[currentState.selectedMoves.size()];
            const std::vector<Simulator::Direction> safeMoves = get_safe_moves(currentState.board, currentPlayerId);
            if (safeMoves.empty()) {
                currentState = suct_update_state(currentState, Simulator::Direction::UP);
            }
            else {
                currentState = suct_update_state(currentState, safeMoves[rng() % safeMoves.size()]);
            }
        }
        return suct_evaluate_state(currentState);
    }

    float suct_ucb(float t_reward, unsigned int t_n, unsigned int t_N) {
        if (t_n == 0) {
            return std::numeric_limits<float>::max();
        }

        return (t_reward / t_n) + /* c * */ std::sqrt(std::log(t_N) / t_n);
    }

    Simulator::Direction suct_select_move(const State& t_state, const std::unordered_map<State, Node>& t_nodes) {
        const std::string& currentPlayerId = t_state.turnOrder[t_state.selectedMoves.size()];

        const std::vector<Simulator::Direction> safeMoves = get_safe_moves(t_state.board, currentPlayerId);
        if (safeMoves.empty()) {
            return Simulator::Direction::UP;
        }
        
        Simulator::Direction bestMove = safeMoves[0];
        float bestMoveUCB = std::numeric_limits<float>::min();
        for (const Simulator::Direction move : safeMoves) {
            const State nextState = suct_update_state(t_state, move);

            auto nodeIt = t_nodes.find(nextState);

            // If we haven't visited this node then UCB value will be +inf
            if (nodeIt == t_nodes.end()) {
                bestMove = move;
                bestMoveUCB = std::numeric_limits<float>::max();
                break;
            }
            
            const RewardMap& rewards = nodeIt->second.rewards;
            const float r = rewards.at(currentPlayerId);
            const unsigned int n = nodeIt->second.visitCount;
            const unsigned int N = t_nodes.at(t_state).visitCount;

            const float ucb = suct_ucb(r, n, N);
            if (ucb > bestMoveUCB) {
                bestMove = move;
                bestMoveUCB = ucb;
            }
        }

        return bestMove;
    }

}
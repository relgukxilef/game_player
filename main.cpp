#include <cstdio>
#include <cstdint>
#include <bit>

bool tic_tac_toe(
    std::uint32_t* state, std::uint32_t* players_input, 
    std::uint32_t* players_score, std::uint32_t* players_see
) {
    uint64_t board = 0b111'111'111;
    int player_o = 0, player_x = 9;
    uint64_t
        vertical = 0b001'001'001,
        horizontal = 0b111,
        diagonal_0 = 0b001'010'100,
        diagonal_1 = 0b100'010'001;

    auto position = *state;
    int score_x = 1; // 1 = draw
    {
        auto p = position;
        for (int player = 0; player < 2; player++) {
            for (int i = 0; i < 3; i++) {
                if (
                    (((p >> i) & vertical) == vertical) |
                    (((p >> i * 3) & horizontal) == horizontal)
                ) {
                    score_x = 2 * player;
                }
            }
            if (
                (p & diagonal_0) == diagonal_0 |
                (p & diagonal_1) == diagonal_1
            ) {
                score_x = 2 * player;
            }
            p >>= player_x;
        }
        players_score[0] = 2 - score_x;
        players_score[1] = score_x;
    }
    
    auto player = std::popcount(position) % 2;

    auto moves = board & ~((position >> player_o) | (position >> player_x));

    if (score_x != 1 || !moves) {
        return false;
    }

    auto move = players_input[player];
    auto bit = 1 << move;

    position = position | ((bit & moves) << (player * player_x));

    {
        auto p = position;
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                if (p & 1) {
                    printf("o");
                } else if (p & (1 << player_x)) {
                    printf("x");
                } else {
                    printf(" ");
                }
                p >>= 1;
            }
            printf("\n");
        }
        printf("\n");
    }

    *state = position;
    players_see[0] = position;
    players_see[1] = position;

    return true;
}

std::uint32_t input_size = 9;

bool update(
    std::uint32_t* state, std::uint32_t* players_input, 
    std::uint32_t* players_score, std::uint32_t* players_see
) {
    return tic_tac_toe(state, players_input, players_score, players_see);
}

struct heuristic {
    struct feature {
        // can't operate on state, as the state needs to contain hidden info too
        // instead it could operate on the last n see values
        // for simplicity, the see value could contain all visible info
        std::uint32_t* mask, * value;
        // separate per player, as they see different states
        std::uint32_t sum, count;
    };
    std::uint32_t history;

    void fit(std::uint32_t* see, std::uint32_t score);
    void estimate(std::uint32_t* see, std::uint32_t& sum, std::uint32_t& count);
};

std::uint32_t random_state = 0;
std::uint32_t random() { return random_state = 0x915f77f5 * random_state + 1; }


void random_play() {
    std::uint32_t state = 0;
    std::uint32_t inputs[2], scores[2], see[2] = {};

    do {
        inputs[0] = random() % 9;
        inputs[1] = random() % 9;
    } while (update(&state, inputs, scores, see));
}

int main() {
    printf("Hello World!\n");
    for (int i = 0; i < 10; i++)
        random_play();
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

typedef struct TYPE_PLAYER_TAG {
    unsigned char starting_skill;
    unsigned char skill;

    unsigned char starting_strength;
    unsigned char strength;

    unsigned char starting_luck;
    unsigned char luck;

    unsigned int gold;
    unsigned int provisions;

    unsigned char equipment_ammount;
    char equipment[128][128];

    unsigned char potions_ammount;
    char potions[128][128];

    unsigned char notes_ammount;
    char notes[128][2048];
} TYPE_PLAYER;

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        // Chop off all of the values that would cause skew...
        long end = RAND_MAX / n; // truncate skew
        assert (end > 0L);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}

void print_player(TYPE_PLAYER player) {
    printf("====================\n");
    printf("Current Skill: %d\n", player.skill);
    printf("Current Strength: %d\n", player.strength);
    printf("Current Luck: %d\n", player.luck);

    printf("Current Gold: %d\n", player.gold);
    printf("Current Provisions: %d\n", player.provisions);

    printf("Potions:\n");
    for(int i = 0; i < player.potions_ammount; i++) {
        printf("(%d)\t%s\n", i + 1, player.potions[i]);
    }

    printf("Equipment:\n");
    for(int i = 0; i < player.equipment_ammount; i++) {
        printf("(%d)\t%s\n", i + 1, player.equipment[i]);
    }

    printf("Notes:\n");
    for(int i = 0; i < player.notes_ammount; i++) {
        printf("(%d)\t%s\n", i + 1, player.notes[i]);
    }
    printf("====================\n");
}

TYPE_PLAYER new_player() {
    TYPE_PLAYER result;

    result.starting_skill = randint(6) + 6;
    result.skill = result.starting_skill;

    result.starting_strength = randint(12) + 12;
    result.strength = result.starting_strength;

    result.starting_luck = randint(6) + 6;
    result.luck = result.starting_luck;

    result.gold = 20;
    result.provisions = 10;

    printf("Select a potion:\n");
    printf("(1)\tPotion of Skill\n");
    printf("(2)\tPotion of Strength\n");
    printf("(3)\tPotion of Luck\n");

    while(result.potions_ammount < 1) {
        char choice;
        scanf(" %c", &choice);

        result.potions_ammount = 1;

        switch(choice) {
            case '1':
                snprintf(result.potions[0], 128, "Potion of Skill");
                break;
            case '2':
                snprintf(result.potions[0], 128, "Potion of Strength");
                break;
            case '3':
                snprintf(result.potions[0], 128, "Potion of Luck");
                break;
            default:
                printf("Invalid choice, please choose 1, 2 or 3\n");
                result.potions_ammount = 0;
                break;
        }
    }

    snprintf(result.equipment[0], 128, "Sword");
    snprintf(result.equipment[1], 128, "Leather Armour");
    result.equipment_ammount = 2;

    result.notes_ammount = 0;

    print_player(result);

    return result;
}

unsigned int fight(TYPE_PLAYER *player, unsigned char enemy_skill, unsigned char enemy_strength) {
    unsigned char player_attack;
    unsigned char enemy_attack;

    for(int round = 1; player->strength > 0 && enemy_strength > 0; round++) {
        player_attack = randint(12) + player->skill;
        enemy_attack = randint(12) + enemy_skill;

        printf("Round %d result:\n", round);

        if(player_attack > enemy_attack) {
            printf("You hurt the enemy\n");
        }
        else if(player_attack < enemy_attack) {
            printf("The enemy hurts you\n");
        }
        else {
            printf("It's a tie\n");
        }

        if(player_attack != enemy_attack){
            char use_luck;
            char life_to_lose = 2;
            printf("Use luck? (y/N)\n");
            scanf(" %c", &use_luck);

            if(use_luck == 'y' || use_luck == 'Y') {
                unsigned char luck_test = randint(12);
                unsigned char got_lucky = player->luck - luck_test;
                player->luck -= 1;

                if(got_lucky >= 0) {
                    if(player_attack > enemy_attack) {
                        life_to_lose += 2;
                    }
                    else {
                        life_to_lose -= 1;
                    }
                    printf("Got lucky!\n");
                }
                else {
                    if(player_attack > enemy_attack) {
                        life_to_lose -= 1;
                    }
                    else {
                        life_to_lose += 1;
                    }
                    printf("Got unlucky!\n");
                }
            }

            if(player_attack > enemy_attack) {
                enemy_strength -= life_to_lose;
            }
            else if(player_attack < enemy_attack) {
                player->strength -= life_to_lose;
            }
        }

        printf("Press a key for next round\n");
        getchar();
    }

    if(player->strength > 0) {
        return 0;
    }
    
    return 1;
}

int main (int argc, char *argv[]) {

    srand(time(NULL));

    TYPE_PLAYER player = new_player();

    fflush(stdin);

    while(1) {
        char option;

        print_player(player);

        printf("\n");
        printf("Menu:\n");
        printf("(1)\tFight\n");
        printf("(2)\tTest Luck\n");
        printf("(3)\tAdd Item\n");
        printf("(4)\tRemove Item\n");
        printf("(5)\tAdd Potion\n");

        scanf(" %c", &option);

        if(option == '1') {
            int enemy_skill, enemy_strength; 
            printf("Enemy skill and strength: \n");
            scanf(" %d %d", &enemy_skill, &enemy_strength);
            if(fight(&player, (unsigned char) enemy_skill, (unsigned char) enemy_strength)) return 0;
        }
        else if(option == '2') {
            unsigned char luck_test = randint(12);
            if(luck_test <= player.luck) {
                printf("Got Lucky!\n");
            }
            else {
                printf("Got unlucky!\n");
            }
            player.luck -= 1;
        }
        else if(option == '3') {
            if(player.equipment_ammount < 128) {
                scanf(" %s", player.equipment[player.equipment_ammount]);
                player.equipment_ammount++;
            }
        }
        else if(option == '4') {
            int item_to_remove;
            printf("Which item to remove?\n");
            scanf(" %d", &item_to_remove);
            memset(player.equipment[item_to_remove-1], 0, 128);
        }
    }

    return 0;
}

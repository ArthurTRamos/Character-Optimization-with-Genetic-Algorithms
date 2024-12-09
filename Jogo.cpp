/*

Definições:

Jogo em que temos personagens com alguns atributos de enfrentando. O objetivo do algoritmo evolutivo é determinar as combinações de stats que criem o melhor personagem.

Atributos:
    Attack (Atk) - Increases the normal damage dealt by a character;
    Defense (Def) - Decreases the normal damage received by a character;
    Health Points (HP) - Increases the amount of damage a character can take before death;
    Regeneration (Regen) - Increases the amount of health points regained by the character every turn;
    Speed (Spd) - Increases the amount of attacks dealt by the character;
    Inteligence (Intel) - Increases the chance of the character doing a special attack;
    Special Attack (SpAtk) - Increases the special damage dealt by a character;
    Special Defense (SpDef) - Decreases the special damage received by a character;
    Luck - Increases the chance of the character dealing a critical hit that increases the damage dealt by an attack;
    Agility (Agi) - Increases the chance of the character to dodge an attack, which nulifies the damage taken.
*/

#include <bits/stdc++.h>

using namespace std;

// Valores base de cada atributo
#define ATK_BASE 0
#define DEF_BASE 0
#define HP_BASE 2800
#define REGEN_BASE 0
#define SPD_BASE 0
#define INTEL_BASE 0
#define SPATK_BASE 0
#define SPDEF_BASE 0
#define LUCK_BASE 0
#define AGI_BASE 0

// Multiplicador para a distribuição dos pontos de cada atributo
#define ATK_MULTIPLIER 2
#define DEF_MULTIPLIER 3
#define HP_MULTIPLIER 5
#define REGEN_MULTIPLIER 1
#define SPD_MULTIPLIER 1
#define INTEL_MULTIPLIER 1
#define SPATK_MULTIPLIER 2
#define SPDEF_MULTIPLIER 3
#define LUCK_MULTIPLIER 1
#define AGI_MULTIPLIER 1

// Outros Multiplicadores de Dano, 1 ponto = +10% de Dano
#define CRIT_DMG 12
#define SPECIAL_MULTIPLIER 10 

// Algums Multiplicadores para a efetividade de alguns status
// Quanto maior o valor, menos efetivo
#define REGEN_EFFECTIVENESS 1 // Efetividade da regeneração
#define AGI_EFFECTIVENESS 1 // Efetividade da agilidade
#define INTEL_EFFECTIVENESS 1 // Efetividade da inteligencia
#define LUCK_EFFECTIVENESS 1 // Efetividade da sorte
#define SPD_EFFECTIVENESS 1 // Efetividade da velocidade

// Multiplicador para a queda de efetividade da regeneração com o passar da batalha
// Quanto maior o valor, mais devagar ela decresce
#define REGEN_DECAY 1
#define SPEED_DECAY 1

// Alguns limites para a aplicação de certos stats
// Influencia apenas no uso do stat em combate, não em sua distribuição
#define AGI_HARD_CAP 99.0 // Limite para a chance de esquiva
#define AGI_SOFT_CAP 60.0 // Ponto em que a chance de esquiva se torna menos efetiva
#define INTEL_CAP 100 // Limite para a chance de realizar um especial
#define CRIT_CAP 100 // Limite para a chance de critico

#define TOTAL_STATS 500 // Total de pontos de stats para uso
#define ATTRIBUTE_NUMBER 10 // Número de atributos do personagem

#define BASE_MUT_RATE 0.01 // Taxa base de mutação
#define MAX_MUT_RATE 0.2 // Taxa Máxima de mutação
#define MIN_VARIATION_EXPECTED 0.01 // Variação esperada a cada geração

#define POP_SIZE 200 // Tamanho da População
#define GENERATION_NUMBER 300 // N° de gerações
#define N_DUELS 3 // N° de duelos em cada confronto
#define N_ANCESTORS 8 // N° de ancestrais salvos
#define GENOCIDE_AMOUNT 70 // Porcentagem da população substituida em um genocidio

#define ENEMY_NUMBER 10

double MUT_RATE = BASE_MUT_RATE; // Inicialização da Taxa de Matação

class Character {
    public:
        int atk;
        int def;
        int hpMax;
        double hpCurr;
        int regen;
        int spd;
        int intel;
        int spAtk;
        int spDef;
        int luck;
        int agi;

        double effectiveAgi;

        vector<int> statPoints;
        int victories;
        double score;
        int index;

        //string name;

        Character() {
            this->index = -1;
        }

        void AttributeStats() {
            this->atk = ATK_BASE + statPoints[0] * ATK_MULTIPLIER;
            this->def = DEF_BASE + statPoints[1] * DEF_MULTIPLIER;
            this->hpMax = HP_BASE + statPoints[2] * HP_MULTIPLIER;
            this->regen = REGEN_BASE + statPoints[3] * REGEN_MULTIPLIER;
            this->spd = SPD_BASE + statPoints[4] * SPD_MULTIPLIER;
            this->intel = INTEL_BASE + statPoints[5] * INTEL_MULTIPLIER;
            this->spAtk = SPATK_BASE + statPoints[6] * SPATK_MULTIPLIER;
            this->spDef = SPDEF_BASE + statPoints[7] * SPDEF_MULTIPLIER;
            this->luck = LUCK_BASE + statPoints[8] * LUCK_MULTIPLIER;
            this->agi = AGI_BASE + statPoints[9] * AGI_MULTIPLIER;

            this->hpCurr = this->hpMax;

            this->victories = 0;
            this->score = 0;

            this->effectiveAgi = this->agi/AGI_EFFECTIVENESS;
            if(this->effectiveAgi <= AGI_SOFT_CAP) {
                this->effectiveAgi = min(AGI_HARD_CAP, this->effectiveAgi); 
            } else {
                this->effectiveAgi = AGI_SOFT_CAP + sqrt(min((AGI_HARD_CAP-AGI_SOFT_CAP)*(AGI_HARD_CAP-AGI_SOFT_CAP), this->effectiveAgi - AGI_SOFT_CAP));
            }
        }

        void GenerateCharacterStats() {
            vector<int> randomPoints;
            int aux = 0;

            for(int i = 0; i < ATTRIBUTE_NUMBER; i++) {
                randomPoints.push_back((int)rand()%ATTRIBUTE_NUMBER);
                aux += randomPoints[i];
            }

            int aux2 = 0;
            

            for(int i = 0; i < ATTRIBUTE_NUMBER; i++) {
                this->statPoints.push_back((int)((randomPoints[i] / aux) * TOTAL_STATS));
                aux2 += this->statPoints[i];
            }

            aux2 = TOTAL_STATS - aux2;

            for(int i = 0; i < aux2; i++) {
                this->statPoints[(int)rand()%ATTRIBUTE_NUMBER]++;
            }

        }

        bool dead() {
            return this->hpCurr <= 0;
        }

        void PrintCharacterStats() {
            cout << "Atk: " << this->atk << " (" << statPoints[0] << ")" << endl;
            cout << "Def: " << this->def << " (" << statPoints[1] << ")" << endl;
            cout << "Hp: " << this->hpMax << " (" << statPoints[2] << ")" << endl;
            cout << "Regen: " << this->regen / REGEN_EFFECTIVENESS << " (" << statPoints[3] << ")" << endl;
            cout << "Spd: " << this->spd / SPD_EFFECTIVENESS << " (" << statPoints[4] << ")" << endl;
            cout << "Intel: " << this->intel / INTEL_EFFECTIVENESS << " (" << statPoints[5] << ")" << endl;
            cout << "SpAtk: " << this->spAtk << " (" << statPoints[6] << ")" << endl;
            cout << "SpDef: " << this->spDef << " (" << statPoints[7] << ")" << endl;
            cout << "Luck: " << this->luck / LUCK_EFFECTIVENESS << " (" << statPoints[8] << ")" << endl;
            cout << "Agi: " << this->agi / AGI_EFFECTIVENESS << " (" << statPoints[9] << ")" << endl;

            cout << "Score: " << this->score << endl;
            cout << "Victories: " << this->victories << endl;
        }

        void TakeDamage(double damage, bool special) {
            int effectiveDef;
            if(special) {
                effectiveDef = this->spDef;
            } else {
                effectiveDef = this->def;
            }

            double dmgTaken = damage - effectiveDef;
            if(dmgTaken < 1) {
                dmgTaken = 1;
            }
            
            bool dodge = (rand()%100 < effectiveAgi);
            if(dodge) {
                dmgTaken = 0;
            }

            this->hpCurr -= dmgTaken;
            if(this->hpCurr < 0) {
                this->hpCurr = 0;
            }
        }

        void FullRegen() {
            this->hpCurr = this->hpMax;
        }

        void RegenerateHP(int turnCount) {
            double effectiveRegen = (this->regen/REGEN_EFFECTIVENESS)/(1 + (turnCount*turnCount/REGEN_DECAY));

            this->hpCurr += effectiveRegen * this->hpMax;

            if(this->hpCurr > this->hpMax) {
                this->hpCurr = this->hpMax;
            }
            //cout << "CURR: " << this->hpCurr << "MAX: " << this->hpMax << endl;
        }

        void Mutation() {

            int mutAmountP, mutAmountN;
            mutAmountP = mutAmountN = MUT_RATE * TOTAL_STATS;

            //cout << "MTRATE" << mutAmountP << endl;

            vector<int> indexes;

            for(int i = 0; i < ATTRIBUTE_NUMBER; i++) {
                indexes.push_back(i);
            }

            random_device rd;
            mt19937 gen(rd());

            shuffle(indexes.begin(), indexes.end(), gen);

            vector<bool> choosen(10, false);

            for(int i = 0; i < ATTRIBUTE_NUMBER and mutAmountP > 0; i++) {
                int increase = rand() % mutAmountP;
                this->statPoints[indexes[i]] += increase;
                mutAmountP -= increase;
                choosen[i] = true;
            }

            this->statPoints[rand() % ATTRIBUTE_NUMBER] += mutAmountP;

            for(int i = 0; i < ATTRIBUTE_NUMBER and mutAmountN > 0; i++) {
                if(!choosen[i]) {
                    int decrease = rand() % mutAmountN;
                    this->statPoints[indexes[i]] -= decrease;
                    mutAmountN -= decrease;
                }   
            }

            int r = rand() % ATTRIBUTE_NUMBER;
            while(mutAmountN > 0) {
                if(this->statPoints[r] < mutAmountN) {
                    mutAmountN -= this->statPoints[r];
                    this->statPoints[r] = 0;
                    r = rand() % ATTRIBUTE_NUMBER;
                } else {
                    this->statPoints[r] -= mutAmountN;
                    mutAmountN = 0;
                }
            }

        }

        bool isSpecial() {
            int effectiveIntel = min(INTEL_CAP, this->intel/INTEL_EFFECTIVENESS);
            return (rand()%100 < effectiveIntel);
        }

        double damageDealt(bool special) {
            int effectiveLuck = min(CRIT_CAP, this->luck/LUCK_EFFECTIVENESS);
            bool cc = (rand()%100 < effectiveLuck);

            double baseDmg;
            if(special) {
                baseDmg = this->spAtk + ((this->spAtk * SPECIAL_MULTIPLIER)/10);
            } else {
                baseDmg = this->atk;
            }

            if(cc) {
                baseDmg = baseDmg + ((baseDmg * CRIT_DMG)/10); 
            }

            return baseDmg;
        }

        void SetIndex(int i) {
            this->index = i;
        }

        void Elitism(Character pai) {
            int aux = TOTAL_STATS;
            for(int i = 0; i < ATTRIBUTE_NUMBER; i++) {
                this->statPoints[i] = (this->statPoints[i] + pai.statPoints[i])/2;
                aux -= this->statPoints[i];
            }

            for(int i = 0; i < aux; i++) {
                this->statPoints[(int)rand()%ATTRIBUTE_NUMBER]++;
            }

            this->Mutation();
            this->AttributeStats();
        }
};

void Attack(Character *player, Character *enemy) {
    int effectiveSpd = player->spd/SPD_EFFECTIVENESS; 

    int number_of_atks = 1;
    while(true) {
        bool doubleAtk = (rand()%100 < effectiveSpd);
        if(doubleAtk) {
            number_of_atks++;
        } else {
            break;
        }
        effectiveSpd /= (number_of_atks*number_of_atks)/SPEED_DECAY;
    }
    

    for(int i = 0; i < number_of_atks; i++) {
        bool special = player->isSpecial();
        enemy->TakeDamage(player->damageDealt(special), special);
    }
}

int Duel(Character *player, Character *enemy) {
    bool first = rand()%2;
    bool inicial = first;
    int turnCount = 0;

    while(true) {
        if(first) {
            player->RegenerateHP(turnCount);
            Attack(player, enemy);
        } else {
            enemy->RegenerateHP(turnCount);
            Attack(enemy, player);
        }
        
        if(first == inicial) {
            turnCount++;
        }
        first = !first;

        if(enemy->dead()) {
            player->victories++;
            break;
        }

        if(player->dead()) {
            enemy->victories++;
            break;
        }
    }

    enemy->FullRegen();
    player->FullRegen();

    return turnCount;
}

vector<Character> Initialize() {
    vector<Character> players(POP_SIZE);
    for(int i = 0; i < POP_SIZE; i++) {
        Character p;
        p.GenerateCharacterStats();
        p.AttributeStats();
        p.SetIndex(i);
        players[i] = p;
    }

    return players;
}

vector<Character> EvaluateTournament(vector<Character> &players) {
    vector<double> fits;
    vector<Character> newPlayers(POP_SIZE);
    vector<pair<double, int>> infos(POP_SIZE); // Pontuação e index;

    fits.assign(POP_SIZE, 0);

    int v;
    for(int i = 0; i < POP_SIZE; i++) {
        for(int j = i + 1; j < POP_SIZE; j++) {
            
            v = players[i].victories;

            int aux = 0;
            for(int k = 0; k < N_DUELS; k++) {
                aux += Duel(&players[i], &players[j]);
            }
            int turns = max(1, aux/N_DUELS);

            if(players[i].victories > v + N_DUELS/2) {
                fits[i] += (players[i].victories)/N_DUELS + POP_SIZE/(turns);
            } else {
                fits[j] += (players[j].victories)/N_DUELS + POP_SIZE/(turns);
            }
            
        }

        fits[i] /= POP_SIZE;
        infos[i] = make_pair(fits[i], players[i].index);
        players[i].score = fits[i];
    }

    sort(infos.begin(), infos.end(), greater<>());

    for(int i = 0; i < POP_SIZE; i++) {
        int indice = infos[i].second;

        newPlayers[i] = players[indice];
        newPlayers[i].SetIndex(i);

    }

    return newPlayers;
}

vector<Character> EvaluateRandom(vector<Character> &players) {
    vector<double> fits;
    vector<Character> newPlayers(POP_SIZE);
    vector<pair<double, int>> infos(POP_SIZE); // Pontuação e index;

    fits.assign(POP_SIZE, 0);

    Character p;
    int v;

    for(int i = 0; i < POP_SIZE; i++) {

        for(int j = 0; j < ENEMY_NUMBER; j++) {
            v = players[i].victories;
            
            p.GenerateCharacterStats();
            p.AttributeStats();

            int aux = 0;
            for(int k = 0; k < N_DUELS; k++) {
                aux += Duel(&players[i], &p);
            }
            int turns = max(1, aux/N_DUELS);

            if(players[i].victories > v + N_DUELS/2) {
                fits[i] += (players[i].victories)/(N_DUELS) + ENEMY_NUMBER/turns;
            }
        }

        fits[i] /= ENEMY_NUMBER;
        infos[i] = make_pair(fits[i], players[i].index);
        players[i].score = fits[i];
    }

    sort(infos.begin(), infos.end(), greater<>());

    for(int i = 0; i < POP_SIZE; i++) {
        int indice = infos[i].second;

        newPlayers[i] = players[indice];
        newPlayers[i].SetIndex(i);

    }

    return newPlayers;
}

vector<Character> Genocide(vector<Character> players) {
    vector<Character> migrated;
    migrated = Initialize();

    int vitimas = GENOCIDE_AMOUNT * POP_SIZE / 100;
    
    for(int i = POP_SIZE - 1; i > POP_SIZE - vitimas; i--) {
        if(i == 0) {
            break;
        }
        players[i] = migrated[i];
    }

    for(int i = 0; i < POP_SIZE; i++) {
        players[i].SetIndex(i);
    }

    //cout << "GENOCIDE!" << endl;
    return players;
}

double popMean(vector<Character> p) {
    double mean = 0;
    for(int i = 0; i < POP_SIZE; i++){
        mean += p[i].score;
    }

    return mean/POP_SIZE;

}

bool Hereditariedade(vector<Character> &antepassados) {
    double scoreMean = 0;
    for(int i = 0; i < N_ANCESTORS; i++) {
        scoreMean += antepassados[i].score;
    }
    scoreMean = scoreMean/N_ANCESTORS;

    //cout << scoreMean << " : " << antepassados[N_ANCESTORS-1].score << " : " << antepassados[N_ANCESTORS-1].score - scoreMean << " : " << scoreMean*MIN_VARIATION_EXPECTED << endl;
    if(abs(antepassados[N_ANCESTORS-1].score - scoreMean) < scoreMean*MIN_VARIATION_EXPECTED) {
        MUT_RATE *= 2;

        if(MUT_RATE > MAX_MUT_RATE) {
            MUT_RATE = BASE_MUT_RATE;
            return true;
        }
    }

    return false;
}

int main(void) {
    srand(time(NULL));

    // Dados para o grafico
    ofstream bestFitnessData("fitness.dat", ofstream::out);
    ofstream popMeanData("mean.dat", ofstream::out);
    ofstream mutData("mut.dat", ofstream::out);

    ofstream atk("atk.dat", ofstream::out);
    ofstream def("def.dat", ofstream::out);
    ofstream hp("hp.dat", ofstream::out);
    ofstream regen("regen.dat", ofstream::out);
    ofstream spd("spd.dat", ofstream::out);
    ofstream intel("intel.dat", ofstream::out);
    ofstream spatk("spatk.dat", ofstream::out);
    ofstream spdef("spdef.dat", ofstream::out);
    ofstream luck("luck.dat", ofstream::out);
    ofstream agi("agi.dat", ofstream::out);

    vector<Character> players;
    vector<Character> evaluated;
    vector<Character> antepassados(N_ANCESTORS);

    Character best;
    Character aux;
    
    players = Initialize();

    int old_percent = 0;

    for(int i = 0; i < N_ANCESTORS; i++) {
        antepassados[i] = players[i];
    }

    for(int i = 0; i < GENERATION_NUMBER; i++) {
        
        evaluated = EvaluateTournament(players);
        best = evaluated[0];

        for(int i = 1; i < N_ANCESTORS; i++){
            antepassados[i-1] = antepassados[i];
        }
        antepassados[N_ANCESTORS-1] = best;

        bestFitnessData << i << " " << best.score << endl;

        mutData << i << " " << MUT_RATE*100 << endl;
        
        atk << i << " " << best.statPoints[0] << endl;
        def << i << " " << best.statPoints[1] << endl;
        hp << i << " " << best.statPoints[2] << endl;
        regen << i << " " << best.statPoints[3] << endl;
        spd << i << " " << best.statPoints[4] << endl;
        intel << i << " " << best.statPoints[5] << endl;
        spatk << i << " " << best.statPoints[6] << endl;
        spdef << i << " " << best.statPoints[7] << endl;
        luck << i << " " << best.statPoints[8] << endl;
        agi << i << " " << best.statPoints[9] << endl;

        // Calcula e salva a media da população
        double playerMean = popMean(evaluated);
        popMeanData << i << " " << playerMean << endl;

        vector<Character> newGeneration(POP_SIZE);

        evaluated[0].victories = 0;
        evaluated[0].score = 0;

        for(int i = 1; i < POP_SIZE; i++) {
            evaluated[i].Elitism(antepassados[0]);
        }

        if(Hereditariedade(antepassados)) {
            Genocide(evaluated);
        }

        players = evaluated;

        int percent = ((i+1)*100)/GENERATION_NUMBER;
        if(percent != old_percent) {
            system("clear");
            cout << percent << "%" << endl;
        }
        old_percent = percent;
    }

    bestFitnessData.close();
    popMeanData.close();
    mutData.close();

    atk.close();
    def.close();
    hp.close();
    regen.close();
    spd.close();
    intel.close();
    spatk.close();
    spdef.close();
    luck.close();
    agi.close();

    system("gnuplot Stats_plot_script.gnu");
    system("gnuplot score_plot_script.gnu");

    best.PrintCharacterStats();

}
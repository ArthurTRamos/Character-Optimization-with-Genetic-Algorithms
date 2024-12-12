# Character Optimization with Genetic Algorithms

In this project, we developed a genetic algorithm capable of determining the optimal attribute conditions for a game character. In this game, two characters face each other in each battle, and they have the following attributes:

  - Attack (Atk) - Increases the normal damage dealt by a character;
  
  - Defense (Def) - Decreases the normal damage received by a character;
  
  - Health Points (HP) - Increases the amount of damage a character can take before death;
  
  - Regeneration (Regen) - Increases the amoult of health points regained by the character every turn;
  
  - Speed (Spd) - Increases the amount of attacks dealt by the character;
  
  - Inteligence (Intel) - Increases the chance of the character doing a special attack;
  
  - Special Attack (SpAtk) - Increases the special damage dealt by a character;
  
  - Special Defense (SpDef) - Decreases the special damage received by a character;
  
  - Luck - Increases the chance of the character dealing a critical hit that increases the damage dealt by an attack;
  
  - Agility (Agi) - Increases the chance of the character to dodge an attack, which nulifies the damage taken.

## Initialization

The initial population is generated with a total of X individuals, where X is a parameter that can be configured for each test. Each individual is a character whose stats are randomly generated based on the total points available.

## Evaluation

In the game, each character fights against every other one in a predefined number of battles. In each battle, the gladiators can attack each other, regenerate their HP, and defend themselves. At the end of the duels between two characters, the one with the most victories is declared the champion of the match and has their fitness increased based on the total number of turns required: the fewer the turns, the higher the fitness, and vice versa. This process provides the evaluation of each individual in the population.

## Selection and Mutation

In the selection and mutation phase, we apply elitism by preserving the best individual of the generation and comparing it to the rest of the population. After that, each new element of the population (except the overall best) has its stats mutated. By the end of this phase, we have a new set of individuals.

## Heredity and Genocide

After each population evaluation, we save the best gladiator at the end of a fixed-size array called ancestors. At the end of each generation, we check if the current best has a higher fitness than the oldest best in the array. If their fitness values are similar, we increment the mutation rate. If it reaches a maximum threshold, we reset it to the base value and apply a genocide, keeping only the best fighter unchanged.

## Graphical Results

Below, there are some graphs showing how the genetic algorithm handles different balances and configurations.

## Video

Link: 

Group
- Arthur Trottmann Ramos - 14681052
- Henrique Drago - 14675441
- João Pedro Boiago Gomes Santana - 14747211

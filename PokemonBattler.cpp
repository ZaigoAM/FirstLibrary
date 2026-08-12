#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>

using namespace std;

class Move{
    public:
        string name;
        string type;
        int power;
        string effect;
        int statchange;

        Move(string n, string t, int p, string e, int sc) : name(n), type(t), power(p), effect(e), statchange(sc) {}
};

class Pokemon{
    private:
        string name;
        string type;
        vector<Move> moveset;
        int hp;
        int maxhp;
        int atk;
        int def;
        int spe;

        void damage(int dmg){
            hp -= dmg;
            if (hp < 0) hp = 0;
        }
        
        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> hpDist;
        uniform_int_distribution<> atkDist;
        uniform_int_distribution<> defDist;
        uniform_int_distribution<> speDist;
        uniform_int_distribution<> crit;
    public:
        Pokemon(string n, string t)
        : name(n), type(t),
          gen(rd()),
          hpDist(200,252),
          atkDist(100,131),
          defDist(100,131),
          speDist(100,131),
          crit(1,6)

        {
            hp = hpDist(gen);
            atk = atkDist(gen);
            def = defDist(gen);
            spe = speDist(gen);
            maxhp = hp;
        }

        void display(){
        cout << name << " (" << type << ")\n";
        this_thread::sleep_for(chrono::milliseconds(800));
        cout << "HP: " << hp << "/" << maxhp << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
        cout << "ATK: " << atk << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
        cout << "DEF: " << def << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
        cout << "SPE: " << spe << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
        }

        void attack(Pokemon &self, Pokemon &target, Move m){
            cout << name << " used " << m.name << "!\n";

            if (m.effect == "DMG"){
                int dmg = atk + m.power - (target.def + target.def%2)/2;
                int roll = crit(gen);
                if (roll == 6){
                    dmg = dmg * 3/2;
                    for (int k = 0; k < 5; k++){
                        cout << "Critical Hit!" << flush;
                        this_thread::sleep_for(chrono::milliseconds(100));
                        cout << "\r" << string(20,' ') << "\r" << flush;
                    }
                    cout << "Critical Hit!" << flush;
                    this_thread::sleep_for(chrono::milliseconds(1500));
                    cout << "\r" << string(20, ' ') << '\r' << flush;
                }
                if (m.type == "Water" && target.type == "Fire") dmg *= 1.5;
                else if (m.type == "Fire" && target.type == "Grass") dmg *= 1.5;
                else if (m.type == "Grass" && target.type == "Water") dmg *= 1.5;
                else if (m.type == "Water" && target.type == "Grass") dmg *= 0.75;
                else if (m.type == "Fire" && target.type == "Water") dmg *= 0.75;
                else if (m.type == "Grass" && target.type == "Fire") dmg *= 0.75;
                if (dmg < 1) dmg = 1;
                target.damage(dmg);
            }
            else if (m.effect == "ATK_DOWN"){
                target.atk -= m.statchange;
                if (target.atk < 40) target.atk = 40; 
                cout << target.name << "'s attack fell!\n";
            }
            else if (m.effect == "ATK_UP"){
                self.atk += m.statchange;
                if (self.atk > 200) self.atk = 200;
                cout << self.name << "'s attack rose!\n";
            }

            else if (m.effect == "DEF_DOWN"){
                target.def -= m.statchange;
                if (target.def < 40) target.def = 40;
                cout << target.name << "'s defense fell!\n";
            }
        }

        void potion(){
            hp += 20;
            if (hp > maxhp) hp = maxhp;
        }

        bool isfainted(){
            if (hp == 0){
                cout << name << " fainted!\n";
                return true;
            }
            return false;
        }
        int getSpeed(){
            return spe;
        }


        void assignmoves(vector<Move> &neutralmovepool, vector<Move> &firemovepool, vector<Move> &watermovepool, vector<Move> &grassmovepool, mt19937 &gen){
            moveset.clear();
            uniform_int_distribution<> nDist(0, neutralmovepool.size() - 1);
            for (int i = 0; i < 2; i++){
                int idx = nDist(gen);
                Move &m = neutralmovepool[idx];
                moveset.push_back(m);
            }
            vector<Move> *typepool;
            if (type == "Fire") typepool = &firemovepool;
            else if (type == "Water") typepool = &watermovepool;
            else if (type == "Grass") typepool = &grassmovepool;
            else typepool = &neutralmovepool;
            uniform_int_distribution<> tDist(0, typepool->size() - 1);

            for (int i = 0; i < 2; i++){
                moveset.push_back((*typepool)[tDist(gen)]);
            }
        }

        void showmoves(){
            cout << "Moves:\n";
            for (int i = 0; i < 4; i++){
                cout << i + 1 << ": " << moveset[i].name << '\n';
            }
        }
        Move getmove(int index){
        return moveset[index];
        }

        int gethp(){
            return hp;
        }
        int getmaxhp(){
            return maxhp;
        }
        string getname(){
            return name;
        }
      
};

int main(){
    random_device rd;
    mt19937 gen(rd());

    vector<Move> neutralmovepool{
        Move("Growl","Normal", 0, "ATK_DOWN", 20),
        Move("Slash","Normal", 30, "DMG", 0),
        Move("Tackle", "Normal", 25, "DMG", 0),
        Move("Leer", "Normal", 0, "DEF_DOWN", 25),
    };
    vector<Move> firemovepool{
        Move("Ember", "Fire", 40, "DMG", 0),
        Move("Flare Blitz", "Fire", 30, "DEF_DOWN", 10 ),
        Move("Flamethrower", "Fire", 50, "DMG", 0)
    };
    vector<Move> watermovepool{
        Move("Water Gun", "Water", 40, "DMG", 0),
        Move("Hydro Pump", "Water", 50, "DMG", 0),
        Move("Scald", "Water", 30, "ATK_DOWN", 10)
    };
    vector<Move> grassmovepool{
        Move("Vine Whip", "Grass", 40, "DMG", 0),
        Move("Solar Beam", "Grass", 60, "DMG", 0),
        Move("Capture", "Grass",10, "ATK_DOWN", 30)
    };

    Pokemon p1("Astile", "Grass");
    Pokemon p2("Dinja", "Water");
    Pokemon p3("Aizard", "Fire");

    p1.assignmoves(neutralmovepool, firemovepool, watermovepool, grassmovepool, gen);
    p2.assignmoves(neutralmovepool, firemovepool, watermovepool, grassmovepool, gen);
    p3.assignmoves(neutralmovepool, firemovepool, watermovepool, grassmovepool, gen);
    
    vector<Pokemon*> pokemons {&p1, &p2, &p3};
    uniform_int_distribution<> dist(0, pokemons.size() - 1);

    int index1 = dist(gen);
    int index2;
    do{
        index2 = dist(gen);
    }while (index1 == index2);
    Pokemon &chosen1 = *pokemons[index1];
    Pokemon &chosen2 = *pokemons[index2];


    vector<string> snake = {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," ","-"," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "," "};
    for (int i = 0; i < snake.size() / 2 ; i++){
        int mid = snake.size() / 2;
        snake[mid + i] = '-';
        snake[mid - i] = '-';
        for (int j = 0; j < snake.size(); j++){
            cout << snake[j] << flush;
        }
        this_thread::sleep_for(chrono::milliseconds(300));
        cout << "\r                    \r" << flush;

    }
    cout << " - - - - - - - - - - POKEMON BATTLER - - - - - - - - - - \n" << flush;
    this_thread::sleep_for(chrono::milliseconds(800));
    cout << "-- -- FROM THE 3 STARTERS, YOUR POKEMON ARE... -- --\n" << flush;
    this_thread::sleep_for(chrono::milliseconds(4000));
    cout << "Player 1's starter:\n";
    chosen1.display();
    chosen1.showmoves();
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "\nPlayer 2's starter:\n";
    chosen2.display();
    chosen2.showmoves();

    while(!chosen1.isfainted() && !chosen2.isfainted()){
        cout << "\nPlayer 1 moves:\n";
        chosen1.showmoves();

        int p1choice;
        cout << "Player 1 choose (1-4 or 5 for potion): ";
        cin >> p1choice;

        cout << "\nPlayer 2 moves:\n";
        chosen2.showmoves();

        int p2choice;
        cout << "Player 2 choose (1-4 or 5 for potion): ";
        cin >> p2choice;

        bool p1first = chosen1.getSpeed() >= chosen2.getSpeed();

        if (p1first) {
            if (p1choice >= 1 && p1choice <= 4)
                chosen1.attack(chosen1, chosen2, chosen1.getmove(p1choice - 1));
            else if (p1choice == 5)
                chosen1.potion();

            if (chosen2.isfainted()) break;

            // Player 2
            if (p2choice >= 1 && p2choice <= 4)
                chosen2.attack(chosen2, chosen1, chosen2.getmove(p2choice - 1));
            else if (p2choice == 5)
                chosen2.potion();
        }
        else{
            if (p2choice >= 1 && p2choice <= 4)
                chosen2.attack(chosen2, chosen1, chosen2.getmove(p2choice - 1));
            else if (p2choice == 5)
                chosen2.potion();

            if (chosen1.isfainted()) break;
            if (p1choice >= 1 && p1choice <= 4)
                chosen1.attack(chosen1, chosen2, chosen1.getmove(p1choice - 1));
            else if (p1choice == 5)
                chosen1.potion();
        }

        cout << "\n" << chosen1.getname() << ':' << chosen1.gethp() << '/' << chosen1.getmaxhp() << '\n';
        cout << '\n'<< chosen2.getname() << ':' << chosen2.gethp() << '/' << chosen2.getmaxhp() << '\n';
        }
    return 0;
}


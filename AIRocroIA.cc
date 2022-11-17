#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME RocroIA


typedef pair<int,Dir> mv_prio;


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  const vector<Dir> dirs = {Up, Down, Left, Right};


  // calculate winning chances

  double win_chance (int me, int enemy) {
    return strength(me) / (strength(me) + strength(enemy));
  }

 /* double ratio_players_vs_total(int me) {

  }
*/


  Dir check_favorable_dirs(int id, int clan_size) {

    priority_queue< mv_prio, vector <mv_prio>, greater<mv_prio> > best_moves;

    for (auto x : dirs) {
        Pos move = unit(id).pos + x;  // current position + tested move

        if (pos_ok(move) and cell(move.i, move.j).type != Waste) {  // check that we can move there
            Cell ch_cell = cell(move.i, move.j);

            int enem = ch_cell.id;

            if (enem == -1) {   // nobody present
                if (ch_cell.food) best_moves.push(mv_prio(2, x));
                else best_moves.push(mv_prio(4, x));
            }
            else {
              if (unit(enem).type == Zombie) {
                  if (clan_size >= 10) best_moves.push(mv_prio(3, x));
                  else best_moves.push(mv_prio(6, x));    // don't go there, not worth risking being in the single digits
              }
              else if (unit(enem).type == Dead) best_moves.push(mv_prio(4, x));
              else {
                int enemPl = unit(enem).player;

                if (win_chance(me(), enemPl) >= 0.6) best_moves.push(mv_prio(1, x));
                else best_moves.push(mv_prio(5, x));
              }
            }
        }
    }
    return best_moves.top().second;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector<int> alive = alive_units(me());    // get a vector with alive player id's
    int clan_size = alive.size();

    for (int id : alive) {
      Dir d = check_favorable_dirs(id, clan_size);
      move(id,d);
    }

  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

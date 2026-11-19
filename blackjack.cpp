#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
struct Card {
  enum Rank {
    r_ace,
    r_2,
    r_3,
    r_4,
    r_5,
    r_6,
    r_7,
    r_8,
    r_9,
    r_10,
    r_jack,
    r_queen,
    r_king,
    max_rank
  };
  enum Suit { clubs, diamonds, hearts, spades, max_suit };
  Rank rank{};
  Suit suit{};

  static constexpr std::array allRanks{r_ace,  r_2,     r_3,   r_4, r_5,
                                       r_6,    r_7,     r_8,   r_9, r_10,
                                       r_jack, r_queen, r_king};
  static constexpr std::array allSuits{clubs, diamonds, hearts, spades};

  static_assert(std::ssize(allRanks) == max_rank, "rank array invalid size");
  static_assert(std::ssize(allSuits) == max_suit, "suit array invalid size");

  int getValue() {
    switch (rank) {
    case r_ace:
      return 11;
    case r_2:
      return 2;
    case r_3:
      return 3;
    case r_4:
      return 4;
    case r_5:
      return 5;
    case r_6:
      return 6;
    case r_7:
      return 7;
    case r_8:
      return 8;
    case r_9:
      return 9;
    case r_10:
    case r_jack:
    case r_queen:
    case r_king:
      return 10;
    default:
      return 0;
    }
  }

  // friend keyword puts this definition outside of the struct definition,
  // which is needed for operator overloads
  //
  // this is defined inside the struct instead of outside so that it can
  // call the enums without prefix
  friend std::ostream &operator<<(std::ostream &out, const Card &card) {
    switch (card.rank) {
    case r_ace:
      out << 'A';
      break;
    case r_2:
      out << '2';
      break;
    case r_3:
      out << '3';
      break;
    case r_4:
      out << '4';
      break;
    case r_5:
      out << '5';
      break;
    case r_6:
      out << '6';
      break;
    case r_7:
      out << '7';
      break;
    case r_8:
      out << '8';
      break;
    case r_9:
      out << '9';
      break;
    case r_10:
      out << "10";
      break;
    case r_jack:
      out << 'J';
      break;
    case r_queen:
      out << 'Q';
      break;
    case r_king:
      out << 'K';
      break;
    default:
      out << "ERROR";
      break;
    }

    switch (card.suit) {
    case clubs:
      out << 'C';
      break;
    case diamonds:
      out << 'D';
      break;
    case hearts:
      out << 'H';
      break;
    case spades:
      out << 'S';
      break;
    default:
      out << "ERROR";
      break;
    }
    return out;
  }
};

int main() {
  // for testing
  for (auto suit : Card::allSuits) {
    for (auto rank : Card::allRanks) {

      std::cout << Card{rank, suit} << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}

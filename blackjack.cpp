#include "Random.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>

namespace Settings {
constexpr int bustLimit{21};
constexpr int dealerLimit{17};
} // namespace Settings

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

  bool isAce() { return rank == r_ace; }

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

class Deck {
  // I'm learning arrays from learncpp for this which is why I use an array
  // even though cards would've probably been better as a vector since it's
  // not constexpr.
private:
  std::array<Card, 52> m_cards{};
  int m_cardIndex{0};

public:
  Deck() {
    int index{0};
    for (auto suit : Card::allSuits) {
      for (auto rank : Card::allRanks) {
        m_cards[index++] = Card{rank, suit};
      }
    }
  }

  Card dealCard() {
    assert(m_cardIndex < std::ssize(m_cards));
    return m_cards[m_cardIndex++];
  }

  void shuffle() {
    std::shuffle(m_cards.begin(), m_cards.end(), Random::mt);
    m_cardIndex = 0;
  }
};

struct Player {
  int score{};
  int aces{0};

  void addCard(Card card) {
    score += card.getValue();
    if (card.isAce()) {
      ++aces;
    }
    // If bust, treat ace as 1 instead of 11
    while (score > Settings::bustLimit && aces > 0) {
      score -= 10;
      --aces;
    }
  }
};

void playGame() {
  Player dealer{0};
  Player player{0};
  Deck deck{};

  deck.shuffle();
  std::cout << '\n' << "The deck is shuffled";

  Card dealerCard1st = deck.dealCard();
  dealer.addCard(dealerCard1st);

  Card playerCard1st = deck.dealCard();
  player.addCard(playerCard1st);
  Card playerCard2nd = deck.dealCard();
  player.addCard(playerCard2nd);

  std::cout << '\n'
            << "Dealer is showing: " << dealerCard1st << " (" << dealer.score
            << ')';
  std::cout << '\n'
            << "You is showing: " << playerCard1st << ' ' << playerCard2nd
            << " (" << player.score << ')';

  bool hitting{true};
  while (hitting && !(player.score > Settings::bustLimit)) {
    char choice{};
    std::cout << '\n' << "(h) to hit, or (s) to stand: ";
    std::cin >> choice;

    // TODO: cleanup and validate cin

    switch (choice) {
    case 'h': {
      Card card = deck.dealCard();
      player.addCard(card);
      std::cout << '\n'
                << "You were dealt " << card << ". "
                << "You now have: " << player.score;
      break;
    }
    case 's': {
      hitting = false;
      break;
    }
    default: {
      std::cout << '\n' << "invalid choice";
      break;
    }
    }
  }

  // Dealer Turn
  while (dealer.score < Settings::dealerLimit &&
         !(player.score > Settings::bustLimit)) { // skip if player busted
    Card card = deck.dealCard();
    dealer.addCard(card);
    std::cout << '\n'
              << "The dealer flips a " << card << ". "
              << "They now have: " << dealer.score;
  }

  // Scoring
  if (player.score > Settings::bustLimit) {
    std::cout << '\n' << "The player went bust!" << '\n' << "You lose!";
  } else if (dealer.score > Settings::bustLimit) {
    std::cout << '\n' << "The dealer went bust!" << '\n' << "You win!";
  } else if (player.score > dealer.score) {
    std::cout << '\n' << "You win!";
  } else if (dealer.score > player.score) {
    std::cout << '\n' << "You lose!";
  } else {
    std::cout << '\n' << "It's a draw!";
  }
}

int main() {
  playGame();
  return 0;
}

#include "EnigmaMachine.hpp"

EnigmaMachine::EnigmaMachine(string& plugboard, vector<string>& rotors)
    : pb(plugboard), rts(rotors.size()), refl() {
  for (size_t i = 0; i < rts.size(); i++) {
    rts[i] = make_shared<Rotor>(rotors[i]);
  }
}

EnigmaMachine::~EnigmaMachine() {}

void EnigmaMachine::receive(const char c) {
  char base_letter;
  int char_no;
  try {
    char_no = getBaseCharacter(base_letter, c);
  } catch (int n) {
    if (n == -1) {
      cout << c;
      return;  // INVALID character left untouched
    }
    cout << "UNKNOWN EXCEPTION OCCURRED" << endl;
    exit(2);
  }

  // FORWARDS DIRECTION OF TRAVEL
  pb.map(char_no);

  // THROUGH ROTORS, ENCODING
  rotor_encode_decode(char_no, true);

  // REFLECTION
  refl.reflect(char_no);

  // THROUGH ROTORS, DECODING
  rotor_encode_decode(char_no, false);

  // BACKWARDS DIRECTION OF TRAVEL
  pb.map(char_no);

  // OUTPUT ENCODED CHARACTER
  cout << char(char_no + base_letter);

  // ROTATE ROTORS
  rotate_rotors();
}

void EnigmaMachine::rotor_encode_decode(int& c, bool encode_decode) {
  vector<shared_ptr<Rotor>>::iterator iter =
      encode_decode ? rts.begin() : rts.end() - 1;

  while (iter >= rts.begin() && iter < rts.end()) {
    (*iter)->encode_decode(c, encode_decode);
    iter += encode_decode ? 1 : -1;
  }
}

void EnigmaMachine::rotate_rotors(void) {
  vector<shared_ptr<Rotor>>::iterator iter = rts.begin();

  while (iter != rts.end() && (*iter)->rotate()) {
    iter++;
  }
}

int EnigmaMachine::getBaseCharacter(char& base_letter, const char c) {
  // Determine letter case and validity
  if (c >= 'A' && c <= 'Z') {
    base_letter = 'A';
    return c - base_letter;
  } else if (c >= 'a' && c <= 'z') {
    base_letter = 'a';
    return c - base_letter;
  }
  throw - 1;
}

#define assert(cond) do{ if(!(cond))die("Assertion failed: " #cond); }while(0)
void die(const char* why);

class IntHolder {
private:
  int x;
public:
  IntHolder(int x_) {
    x = x_;
  }

  operator bool() {
    return x;
  }

  inline bool operator==(const IntHolder& other) {
    return x == other.x;
  }

  IntHolder& operator=(const IntHolder& other) {
    x = other.x;
    return *this;
  }
};

void f(int x) {
  if (x = 3) { // BAD
  }
  if ((x = 3)) { // GOOD: explicitly bracketed
  }
  if (!(x = 3)) { // BAD
  }
  if (!((x = 3))) { // GOOD: explicitly bracketed
  }
  do {
  } while (x = 0); // BAD
  do {
  } while ((x = 0)); // GOOD: explicitly bracketed
  if ((x = 3) && (x = 4)) { // BAD (x2)
  }
  if (((x = 3)) && ((x = 4))) { // GOOD: explicitly bracketed
  }
  x = (x = 3) ? 2 : 1; // BAD
  x = ((x = 3)) ? 2 : 1; // GOOD: explicitly bracketed
  assert(x = 2); // BAD
  assert((x = 2)); // GOOD: explicitly bracketed

  int y;

  if (y = 1) { // GOOD: y was not initialized so it is probably intentional.
  }
  y = 2;
  if (y = 3) { // BAD: y has been initialized so it is probably a mistake.
  }

  int z = 1;

  if (z = 2) { // BAD: z has been initialized so it is probably a mistake.
  }
  IntHolder holder1(x);
  IntHolder holder2(x);
  holder1 = holder2;
  holder1 == holder2;
  if(holder1 = holder2) { // BAD: holder is initialized [FALSE NEGATIVE]
  }
  if(holder1 == holder1) {
  }
}

int global;

void g(int *i_p, int cond) {
  int i, j, k, x, y;
  static int s, t = 0;

  if (global = 0) { // BAD: this is unlikely to be a deliberate initialization of global
  }
  if (*i_p = 0) { // BAD
  }
  if (s = 0) { // BAD
  }
  if (s = 0) { // BAD
  }
  if (t = 0) { // BAD
  }

  for (i = 0, j = 0; i < 10; i++) { // GOOD
    if (x = i) { // GOOD: x was not initialized the first time around the loop
    }
  }

  for (k = 0; !(k = 10); k++) { // BAD
  }

  if (cond) {
    y = 1;
  }
  if (y = 1) { // GOOD: y might not be initialized so it is probably intentional.
  }
}
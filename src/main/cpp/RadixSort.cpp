#include <assert.h>
#include <iostream>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

using namespace std;

static const uint32_t MAX_RANDOM = 10000;
static const bool VERIFY = true;

static inline uint32_t clz(uint32_t i) {
  if (i == 0) {
    return sizeof(i) * CHAR_BIT;
  }
  uint32_t r = 0;
  if (!(i & 0xFFFF0000)) {
    r += 16;
    i <<= 16;
  }
  if (!(i & 0xFF000000)) {
    r += 8;
    i <<= 8;
  }
  if (!(i & 0xF0000000)) {
    r += 4;
    i <<= 4;
  }
  if (!(i & 0xC0000000)) {
    r += 2;
    i <<= 2;
  }
  if (!(i & 0x80000000)) {
    r += 1;
  }
  return r;
}

void sort(vector<uint32_t> &v, uint32_t bitWidth) {
  const uint32_t remMask = ~(~0 << bitWidth);
  uint32_t max = 0;
  for (vector<uint32_t>::const_iterator it = v.begin(); it != v.end(); it++) {
    if (*it > max) {
      max = *it;
    }
  }
  const uint32_t hb = sizeof(max) * CHAR_BIT - clz(max);
  vector<uint32_t> c(1 << bitWidth); // count array
  vector<uint32_t> b(v.size());

  // count sort based on radix
  //
  for (uint32_t e = 0; e < hb; e += bitWidth) {
    fill(c.begin(), c.end(), 0); // zero counts
    for (vector<uint32_t>::size_type i = 0; i < v.size(); i++) {
      c[(v[i] >> e) & remMask]++;
    }
    for (vector<uint32_t>::size_type i = 1; i < c.size(); i++) {
      c[i] += c[i - 1];
    }
    for (vector<uint32_t>::size_type i = v.size() - 1;
         i != (vector<uint32_t>::size_type)~0;
         i--)
    {
      b[--c[(v[i] >> e) & remMask]] = v[i];
    }
    v = b;
  }
}

static vector<uint32_t>::size_type binSearchR(
  vector<uint32_t> &v,
  vector<uint32_t>::size_type s,
  vector<uint32_t>::size_type e,
  uint32_t k)
{
  // check argument validity
  if (s > e
   || s >= v.max_size()
   || e >= v.max_size())
  {
    return v.max_size();
  }

  vector<uint32_t>::size_type m = (s + e) >> 1;
  if (k < v[m]) {
    return binSearchR(v, s, m - 1, k);
  } else if (k > v[m]) {
    return binSearchR(v, m + 1, e, k);
  }
  return m;
}

vector<uint32_t>::size_type searchKey(vector<uint32_t> &v, uint32_t k) {
  return binSearchR(v, 0, v.size(), k);
}

static void genv(vector<uint32_t> &v) {
  for (vector<uint32_t>::iterator it = v.begin(); it != v.end(); it++) {
    *it = 1 + rand() % MAX_RANDOM;
  }
}

static void printv(const char header[], vector<uint32_t> &v) {
  cout << header << endl;
  for (vector<uint32_t>::const_iterator it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";
  }
  cout << endl;
}

int main(int argc, char *argv[]) {
  vector<uint32_t>  v(1000);
  genv(v);
  vector<uint32_t> preSort;
  vector<uint32_t> postSort;
  if (VERIFY) {
    preSort = v;
    postSort = v;
    std::sort(postSort.begin(), postSort.end());
    assert (!equal(preSort.begin(), preSort.end(), postSort.begin()));
  }
  sort(v, 8);
  if (VERIFY) {
    // v[0] = -1;
    if (!equal(v.begin(), v.end(), postSort.begin())) {
      cout << "RadixSort broken" << endl;
      printv("Generated", preSort);
      printv("Reference", postSort);
      printv("RadixSort", v);
    } else {
      cout << "SUCCESS: RadixSort finished and verified" << endl;
    }
  }

  // Test 1: existing key is findable
  vector<uint32_t>::size_type randPos = rand() % v.size();
  uint32_t randKey = v[randPos];
  uint32_t foundPos = searchKey(v, randKey);
  assert (v[foundPos] == randKey);
  cout << "SUCCESS: binSearch " << randKey << " at " << foundPos;
  cout << ", randPos=" << randPos << endl;

  // Test 2: fail to find outside the value range
  assert (searchKey(v, MAX_RANDOM + 1) == v.max_size());
  cout << "SUCCESS: binSearch not found " << MAX_RANDOM << endl;
  assert (searchKey(v, 0) == v.max_size());
  cout << "SUCCESS: binSearch not found " << 0 << endl;

  // Test 3: fail to find non-existing key withing the range
  uint32_t nek = 0; // non-existing key
  for (vector<uint32_t>::size_type i = 1; i < v.size(); i++) {
    if (v[i] - v[i - 1] > 1) {
      nek = v[i] - 1;
      break;
    }
  }
  if (nek > 0) {
    assert (searchKey(v, nek) == v.max_size());
    cout << "SUCCESS: binSearch not found " << nek << endl;
  }
}

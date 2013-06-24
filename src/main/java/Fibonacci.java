public class Fibonacci {
  // Recursive definition
  //
  public int getR(int n) {
    if (n == 0 || n == 1) {
      return n;
    }
    return getR(n - 2) + getR(n - 1);
  }

  // get(0): rc[0]                 // 0
  // get(1): rc[1]                 // 1
  // get(2): rc[0] = rc[0] + rc[1] // 1
  // get(3): rc[1] = rc[0] + rc[1] // 2
  // ...
  //
  public int get(int n) {
    final int[] rc = new int[] {0, 1};
    for (int i = 2; i <= n; i++) {
      rc[i % 2] = rc[0] + rc[1];
    }
    return rc[n % 2];
  }

  public static void main(String[] args) {
    final Fibonacci f = new Fibonacci();

    boolean assertWorks = false;
    try {
      assert 1 == 0;
    } catch (AssertionError e) {
      assertWorks = true;
    }
    if (!assertWorks) {
      System.err.println("Unit tests are not enabled");
      System.exit(-1);
    }
    assert f.getR(0) == 0;
    assert f.getR(1) == 1;
    assert f.getR(2) == 1;
    assert f.getR(3) == 2;
    assert f.getR(4) == 3;

    assert f.getR(0) == f.get(0);
    assert f.getR(1) == f.get(1);
    assert f.getR(2) == f.get(2);
    assert f.getR(3) == f.get(3);
    assert f.getR(4) == f.get(4);

    assert f.getR(19) == f.get(19);
    assert f.getR(20) == f.get(20);

    System.out.println("Fib(20)=" + f.get(20));
  }
}

class Main {
	static double calculate(int num1, int num2) {
		return (double)num1 / (double)num2;
	}

	private static void say_hello(int number) {
		System.out.print("Hello, world!");
		System.out.println(number);

		if(number % 2 == 0) {
			System.out.println(calculate(number, number - 1));
		}
	}

	public static void Main(String[] args) {
		for(int i = 0; i < 3; i++) {
			say_hello(i + 1);
		}
	}
}

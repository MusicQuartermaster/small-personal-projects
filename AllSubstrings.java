import java.util.HashSet;
import java.util.Scanner;

public class AllSubstrings {

	// iteratively prints every single substring within a given string

	public static void main(String[] args) {
		// user input
		Scanner input = new Scanner(System.in);
		System.out.println("Enter a string:");
		String s = input.nextLine();
		input.close();

		// use hashset to ignore duplicate substrings
		HashSet<String> substrings = new HashSet<>();

		// store each individual substring together into one large string
		// output looks nicer than iterating through the hashset keys
		String resList = "";
		for (int start = 0; start < s.length(); start++) {
			for (int end = s.length(); end > start; end--) {
				String substring = s.substring(start, end);
				if (!substrings.contains(substring)) {
					resList += substring + "\n";
					substrings.add(substring);
				}
			}
		}
		System.out.println(resList);
	}
}

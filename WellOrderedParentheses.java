import java.util.ArrayList;
import java.util.List;

public class WellOrderedParentheses {
	// coding challenge:
	// https://www.geeksforgeeks.org/problems/generate-all-possible-parentheses/1

	// prints out all permutations of N well-ordered parentheses

	int n = 3;

	public static void main(String[] args) {
		new WellOrderedParentheses();
	}

	public WellOrderedParentheses() {
		System.out.println(generateParenthesis(n));
	}

	// start recursion and return results
	public List<String> generateParenthesis(int n) {
		List<String> list = new ArrayList<>();
		genParen("", n, n, list);
		return list;
	}

	// recursive function
	private void genParen(String s, int open, int close, List<String> list) {
		// base case
		if (open == 0 && close == 0) {
			list.add(s);
			return;
		}

		// once we reach the max number of opening parentheses, fill string with the
		// remaining closing parentheses
		if (open == 0) {
			genParen(s + ")".repeat(close), open, 0, list); // .repeat(count) only works with Java 11 and later
			return;
		}

		if (open == close) {
			// if we "run out" of closing parentheses, we most use an opening parenthesis
			// next
			genParen(s + "(", open - 1, close, list);
		} else {
			genParen(s + "(", open - 1, close, list);
			genParen(s + ")", open, close - 1, list);
		}
	}
}
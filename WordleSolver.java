import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Scanner;

public class WordleSolver {
	// store each letter in a char to make altering code easier
	static final char A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', J = 'J', K = 'K',
			L = 'L', M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T', U = 'U', V = 'V', W = 'W',
			X = 'X', Y = 'Y', Z = 'Z';

	static final File words = new File(".\\word_list.txt");
	// set of valid Wordle words indexed by the first character
	static final ArrayList<HashSet<String>> wordList = new ArrayList<>(26);

	public static void main(String[] args) throws FileNotFoundException {
		// create 26 "buckets" that will each store a hashset of words that start with
		// the given letter
		for (int i = 'A'; i <= 'Z'; i++) {
			wordList.add(new HashSet<String>());
		}

		// load word list into program
		Scanner fileReader = new Scanner(words);
		while (fileReader.hasNext()) {
			String word = fileReader.next().toUpperCase();
			// place each word into the bucket that corresponds with the first letter of the
			// word
			wordList.get(word.charAt(0) - 'A').add(word);
		}
		fileReader.close();

		// the letters that could be or definitely are in the final word
		char[] availableLetters = new char[] { Q, W, P, A, D, I, F, H, L, K, Z, X, V, N, M };
		// the list of words that fit the given criteria
		HashSet<String> potentialWordList = new HashSet<>();
		// letters that already have a correct position
		char[] placedLetters = { 0, 0, N, A, 0 };
		// letters that are definitely in the word
		char[] containedLetters = { N, A };

		printWords(placedLetters, potentialWordList, availableLetters, containedLetters);
	}

	// begins recursive method
	public static void printWords(char[] placedLetters, HashSet<String> potentialWordList, char[] availableLetters,
			char[] containedLetters) {
		printWords(placedLetters, 0, potentialWordList, availableLetters, containedLetters);
	}

	// recrusively tries each available letter in each available slot to search for
	// valid words
	public static void printWords(char[] placedLetters, int idx, HashSet<String> potentialWordList,
			char[] availableLetters, char[] containedLetters) {

		// once all placements have been made
		if (idx >= placedLetters.length) {
			String potentialWord = new String(placedLetters);
			// if we encounter a new (valid) word, and if that word contains all required
			// letters
			if (!potentialWordList.contains(potentialWord) && containsLetters(placedLetters, containedLetters)
					&& wordList.get(placedLetters[0] - 'A').contains(potentialWord)) {
				// then add the word to our set and print it out
				potentialWordList.add(potentialWord);
				System.out.println(placedLetters);
			}
			return;
		}

		// if there is already a letter at the current location, move to the next index
		if (placedLetters[idx] != 0) {
			printWords(placedLetters, idx + 1, potentialWordList, availableLetters, containedLetters);
		} else {
			// try each letter from the list of available letters
			for (char letter : availableLetters) {
				placedLetters[idx] = letter;
				printWords(placedLetters, idx + 1, potentialWordList, availableLetters, containedLetters);
			}
			placedLetters[idx] = 0;
		}
	}

	// returns true if each letter in containedLetters also appears in word
	public static boolean containsLetters(char[] word, char[] containedLetters) {
		outer: for (char c : containedLetters) {
			for (int i = 0; i < word.length; i++) {
				if (word[i] == c) {
					continue outer;
				}
			}
			// if the inner loop exits successfully, no match for the given letter c was
			// found
			return false;
		}
		return true;
	}

}

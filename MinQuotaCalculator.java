package lethal_company;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

/*
 * this is a simple script I made to help with a game I've been playing with friends called "Lethal Company"
 * where you have to explore abandoned warehouses looking for scrap. the point of this script is to show which
 * items to sell to meet the quota so that we'll have the most amount of value to meet the next quota
 */

public class MinQuotaCalculator {
	// all the items
	static Integer[] itemArray;
	// currently-selected items
	static boolean[] selected;
	// selected items that minimize going over the quota
	static boolean[] minSelected;
	// the minimum value that exceeds the quota
	static int min = Integer.MAX_VALUE;

	public static void main(String[] args) {
		// get user input
		Scanner input = new Scanner(System.in);
		System.out.println("Enter total quota: ");
		int total = input.nextInt();
		System.out.println("Enter current value reached: ");
		int target = total - input.nextInt();

		System.out.println("Enter item values separated by a space. Enter 0 when done");
		ArrayList<Integer> items = new ArrayList<>();
		int itemCount = 0;
		int next = input.nextInt();
		while (next != 0) {
			items.add(next);
			itemCount++;
			next = input.nextInt();
		}

		System.out.println(
				"Would you like to prioritize high-value items (fewer items sold) or low-value items (more items sold)?");
		System.out.println("0: high-value");
		System.out.println("1: low-value");
		boolean prioritizeHighVal = input.nextInt() == 0;
		input.close();

		// store item values into an array and sort
		itemArray = new Integer[itemCount];
		items.toArray(itemArray);
		Arrays.sort(itemArray);

		// if the user wants to prioritize high-value items (and therefore fewer items
		// sold) then reverse the sorted list
		if (prioritizeHighVal)
			for (int i = 0; i < itemArray.length / 2; i++) {
				int temp = itemArray[i];
				itemArray[i] = itemArray[itemArray.length - i - 1];
				itemArray[itemArray.length - i - 1] = temp;
			}

		// calculate and print results
		closestSum(target);
		printResults();
	}

	// recursive helper method
	public static int closestSum(int target) {
		selected = new boolean[itemArray.length];
		minSelected = new boolean[itemArray.length];
		return closestSum(target, 0, 0);
	}

	// recursively try all permutations until an exact match is found
	public static int closestSum(int target, int sum, int idx) {
		/* --- BASE CASES --- */

		// if exact quota is met, store the selected list for later
		if (target == sum) {
			minSelected = selected.clone();
			min = 0;
			printResults();
		}

		// if quota is exceeded, check if it is smaller than the current minimum
		if (sum > target && sum - target < min) {
			minSelected = selected.clone();
			min = sum - target;
		}

		// if end of list is reached, either return the positive difference or infinity
		if (idx >= itemArray.length) {
			if (sum >= target)
				return sum - target;
			else
				return Integer.MAX_VALUE;
		}

		/* --- RECURSIVE CASES --- */

		// try adding the value to the quota
		selected[idx] = true;
		int take = closestSum(target, sum + itemArray[idx], idx + 1);

		// try skipping the value
		selected[idx] = false;
		int leave = closestSum(target, sum, idx + 1);

		// return the smaller of the two
		return Math.min(take, leave);
	}

	private static void printResults() {
		String res = "[";
		for (int i = 0; i < itemArray.length; i++) {
			if (minSelected[i])
				res += itemArray[i] + ", ";
		}
		if (res.contains(","))
			res = res.substring(0, res.lastIndexOf(","));
		res += "]";
		if (res.equals("[]")) {
			System.out.println("The quota is unreachable");
		} else {
			System.out.println("Min over: " + min);
			System.out.println(res);
		}
		System.exit(0);
	}

}

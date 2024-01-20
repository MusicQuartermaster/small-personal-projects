import java.util.Scanner;

public class ValorantBattlepassCalculator {

	// calculates the amount of experience points (XP) required to unlock all the
	// items in Valorant depending on the number of items you already have unlocked,
	// as well as the amount of XP required to earn per day according to how much
	// time is left until the items reset

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);

		System.out.println("Enter your current level: ");
		int currentLevel = input.nextInt() - 1;
		System.out.println("Enter current xp for your level: ");
		int currentRemaining = input.nextInt();
		System.out.println("Enter remaining days until BP ends: ");
		int daysRemaining = input.nextInt();

		int weeklyMissionsPerWeek = 3;
		int numOfWeeklies = (daysRemaining / 7) * weeklyMissionsPerWeek;
		System.out.println("Remaining number of weeklies: " + numOfWeeklies);

		int XPPerDay;
		{
			// XP received per weekly mission
			int weeklyAmount = 15000;
			// XP to unlock the first tier
			int initialTierXP = 1250;
			int xpPerTier = 750;
			// XP received per daily mission
			int dailyXP = 4000;
			int totalTiers = 50;

			// subtract all weekly XP
			int totalXPRemaining = currentRemaining - (numOfWeeklies * weeklyAmount);

			// add the XP values of all remaining tiers
			for (int i = currentLevel; i < totalTiers; i++) {
				int current = i * xpPerTier + initialTierXP;
				totalXPRemaining += current;
			}

			XPPerDay = (int) (totalXPRemaining / daysRemaining) - dailyXP;
		}

		System.out.println(
				String.format("You have %d XP left, and %d day(s) to earn it, meaning you must earn %d XP per day.",
						XPPerDay * daysRemaining, daysRemaining, XPPerDay));
		input.close();
	}
}
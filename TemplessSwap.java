import java.util.Arrays;

public class TemplessSwap {

	// proof of concept of a swap algorithm that doesn't use a temp variable

	// unfortunately, only works with integers (and therefore chars) and floats with
	// a fixed precision so that errors can be rounded away

	// this method is also prone to over- and underflow errors
	public static void main(String[] args) {
		int[] arr = { -1, 0, 3, 4, 5 };
		swap(arr, 0, 1);
		System.out.println(Arrays.toString(arr));
	}

	public static void swap(int[] arr, int i, int j) {
		arr[i] += arr[j];
		arr[j] = arr[i] - arr[j];
		arr[i] -= arr[j];
	}
}

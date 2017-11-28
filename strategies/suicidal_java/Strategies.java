import java.util.Scanner;

class Strategies {
	public static void main(String[] args) {
		Scanner s = null;
		Boolean isAnt = false;
		try {
			s = new Scanner(System.in);
			while (s.hasNextLine()) {
				String line = s.nextLine();
				if (line.equals("END")) {
					if (isAnt) {
						System.out.println("SUICIDE");
						isAnt = false;
					}
					System.out.println("END");
					System.out.flush();
				} else if (line.equals("BEGIN ANT")) {
					isAnt = true;
				}
			}
		} finally {
			if (s != null) {
				s.close();
			}
		}
	}
}

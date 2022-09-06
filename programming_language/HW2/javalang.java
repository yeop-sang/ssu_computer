package programingLang.hw2;

import java.io.*;
import java.util.Scanner;
import java.lang.IndexOutOfBoundsException;

public class JavaLang {

	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		try {
			RDParser p = new RDParser();
			while (true) {
				System.out.print(">> ");
				String tempString = sc.nextLine();
				if (tempString.length() == 0)
					break;
				p.parse(tempString);
			}
		} catch (ParsingException e) {
			System.out.println("syntax Error!!");
		} finally {
			sc.close();
		}
	}

}

// 4531 + (32* 5) + (86 + 5) == 4782

class ParsingException extends Exception {
	public ParsingException() {
		super("syntax error!");
	}

	public ParsingException(String str) {
		super(str);
	}
}

class RDParser {
	private String input;
	private int idx = 0;

	public void parse(String i) throws ParsingException {
		input = new String(i);
		// 마지막을 뜻하는 특수기호 삽입
		input += "$";
		idx = 0;
		long res = expr();
		if (input.charAt(idx) != '$')
			throw new ParsingException("parsing is not ended!");
		System.out.printf("%d\n", res);
	}

	private void ffSpace() {
		while (input.charAt(idx) == ' ' || input.charAt(idx) == '\t')
			idx++;
	}

	private boolean is_valid(char a) {
		switch (a) {
			case '+':
			case '-':
			case '*':
			case '/':
			case '(':
			case ')':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ' ':
			case '\t':
			case '$':
				return true;
			default:
				return false;
		}
	}

	private int digit() throws ParsingException{
		char c = input.charAt(idx);
		if(!is_valid(c))
			throw new ParsingException();

		return c - '0';
	}

	private long number() throws ParsingException {
		long result = 0;
		int temp_digit = 0;

		ffSpace();

		while (true) {
			// 숫자가 아닌 문자가 오면 종료
			if ((temp_digit = digit()) < 0 || temp_digit > 9)
				break;

			result = result * 10 + temp_digit;
			idx++;
		}

		return result;
	}

	private long factor() throws ParsingException {
		long result = 0;

		ffSpace();

		if (input.charAt(idx) == '(') {
			idx++;
			ffSpace();
			result = expr();
			ffSpace();
			// ) 가 없으면 오류!
			if (input.charAt(idx) != ')')
				throw new ParsingException();
			idx++;
		} else {
			if (input.charAt(idx) == '-') {
				result = -1;
				idx++;
			} else {
				result = 1;
			}

			result = result * number();
		}

		return result;
	}

	private long term() throws ParsingException {
		long result = factor();

		ffSpace();

		while (true) {
			char operator = input.charAt(idx);

			// operator가 * 이나 /인 경우만 처리한다.
			if (operator == '*' || operator == '/') {
				idx++;
				ffSpace();
				if (operator == '*')
					result *= factor();
				else if (operator == '/')
					result /= factor();
			} else
				// 아니면 종료
				break;
		}
		return result;
	}

	private long expr() throws ParsingException {
		long result = term();

		ffSpace();

		while (true) {
			char operator = input.charAt(idx);

			// operator가 + 나 - 인 경우에만 처리한다.
			if (operator == '+' || operator == '-') {
				idx++;
				ffSpace();

				if (operator == '+')
					result += term();
				else if (operator == '-')
					result -= term();
			} else
				// 아니면 종료
				break;
		}

		return result;
	}
}

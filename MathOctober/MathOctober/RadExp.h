#ifndef RADEXP_H
#define RADEXP_H

#include <string>
#include <vector>
#include <map>
#include <cmath>

struct RadExpPiece
	{
	RadExpPiece();
	RadExpPiece(char,int);
	char t;
	// 'c' means RadExp::constants
	// 'v' means RadExp::variables
	// 'o' means RadExp::operators
	// 'e' means RadExp::subExpressions
	int i;
	std::string toString();
	};

class RadExp
	{
	public:
		RadExp();
		RadExp(std::string);
		RadExp(std::string,RadExp*);
		double evaluate(std::map<std::string,double>);
		std::string toString();
		void buildFromString(std::string);

	private:
		RadExp* parentExpression;
		std::vector<std::string> variables;
		std::vector<double> constants;
		std::vector<char> operators;
		std::vector<RadExp> subExpressions;
		std::vector<RadExpPiece> pieces;
	};

#endif // RADEXP_H
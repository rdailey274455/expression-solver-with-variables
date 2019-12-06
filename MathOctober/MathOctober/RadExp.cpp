#ifndef RADEXP_CPP
#define RADEXP_CPP

#define CHAR_IS_NUM(c) ((c>='0' && c<'9') || c=='.')
#define CHAR_IS_OP(c) (c=='^' || c=='-' || c=='*' || c=='/' || c=='+')
#include "RadExp.h"

RadExpPiece::RadExpPiece()
	{
	RadExpPiece::RadExpPiece(' ',-1);
	}

RadExpPiece::RadExpPiece(char t,int i)
	{
	this->t=t;
	this->i=i;
	}

std::string RadExpPiece::toString()
	{
	return "["+std::string(1,this->t)+", "+std::to_string(this->i)+"]";
	}

RadExp::RadExp()
	{
	this->parentExpression=nullptr;
	this->buildFromString("");
	}

RadExp::RadExp(std::string inStr)
	{
	this->parentExpression=nullptr;
	this->buildFromString(inStr);
	}

RadExp::RadExp(std::string inStr,RadExp* pExp)
	{
	this->parentExpression=pExp;
	this->buildFromString(inStr);
	}

void RadExp::buildFromString(std::string inStr)
	{
	this->constants.clear();
	this->variables.clear();
	this->operators.clear();
	this->subExpressions.clear();
	for (int ci=0; ci<inStr.length(); )
		{
		if (CHAR_IS_NUM(inStr[ci])) // constants
			{
			int cj=-1;
			for (cj=ci; cj<inStr.length(); cj++)
				{
				if (!CHAR_IS_NUM(inStr[cj]))
					{
					break;
					}
				}
			this->constants.push_back(std::stod(inStr.substr(ci,cj-ci)));
			this->pieces.push_back(RadExpPiece('c',this->constants.size()-1));
			ci=cj;
			}
		else if (inStr[ci]=='(') // subexpressions
			{
			int cj=ci;
			while (inStr[cj]!=')')
				{
				cj++;
				}
			//debug!
			//std::cout<<"Subexpression: "<<inStr.substr(ci+1,cj-(ci+1)); //<<this->subExpressions[this->subExpressions.size()-1];
			//debug!
			this->subExpressions.push_back(RadExp(inStr.substr(ci+1,cj-ci),this));
			this->pieces.push_back(RadExpPiece('e',this->subExpressions.size()-1));
			ci=cj;
			}
		else if (inStr[ci]==')')
			{
			ci++;
			continue;
			}
		else if (CHAR_IS_OP(inStr[ci])) // operators
			{
			this->operators.push_back(inStr[ci]);
			this->pieces.push_back(RadExpPiece('o',this->operators.size()-1));
			ci++;
			}
		else // variables
			{
			int cj=-1;
			for (cj=ci; cj<inStr.length(); ++cj)
				{
				if (CHAR_IS_NUM(inStr[cj])||CHAR_IS_OP(inStr[cj])||inStr[cj]=='('||inStr[cj]==')')
					{
					break;
					}
				}
			this->variables.push_back(inStr.substr(ci,cj-ci));
			this->pieces.push_back(RadExpPiece('v',this->variables.size()-1));
			ci=cj;
			}
		}
	}


double RadExp::evaluate(std::map<std::string,double> varVals)
	{
	std::vector<double> nums;
	std::vector<char> ops;

	// debug!
	//std::cout<<std::endl<<"Count of pieces: "<<std::to_string(this->pieces.size())<<std::endl;
	// debug!


	for (int pi=0; pi<this->pieces.size(); pi++)
		{
		char pieceType=(this->pieces[pi]).t;
		int pieceIndex=(this->pieces[pi]).i;

		// debug!
		//std::cout<<"Piece type: "+std::string(1,pieceType)+"; Piece index: "+std::to_string(pieceIndex)<<std::endl;
		// debug!


		switch (pieceType)
			{
			case 'v':
				nums.push_back(varVals[this->variables[pieceIndex]]);
				break;
			case 'o':
				ops.push_back(this->operators[pieceIndex]);
				break;
			case 'e':
				nums.push_back((this->subExpressions[pieceIndex]).evaluate(varVals));
				break;
			case 'c':
				nums.push_back(this->constants[pieceIndex]);
				break;
			default:
				break;
			}
		}

	// debug!
	/*
	std::cout<<std::endl;
	std::cout<<"nums: ";
	for (int i=0; i<nums.size(); ++i) { std::cout<<nums[i]<<((i<nums.size()-1)?", ":""); }
	std::cout<<std::endl;
	std::cout<<"ops: ";
	for (int i=0; i<ops.size(); ++i) { std::cout<<ops[i]<<((i<ops.size()-1)?", ":""); }
	std::cout<<std::endl;
	std::cout<<std::endl;
	*/
	// debug!

	for (int emdas=0; emdas<=2; ++emdas)
		{
		for (int oi=0; oi<ops.size(); oi++)
			{
			bool opDone=false;
			if (emdas==0&&ops[oi]=='^') // exponents
				{
				nums[oi]=pow(nums[oi],nums[oi+1]);
				opDone=true;
				}
			else if (emdas==1&&(ops[oi]=='/'||ops[oi]=='*')) // multiplication and division
				{
				nums[oi]=(ops[oi]=='*'?nums[oi]*nums[oi+1]:nums[oi]/nums[oi+1]);
				opDone=true;
				}
			else if (emdas==2&&(ops[oi]=='+'||ops[oi]=='-')) // addition and subtraction
				{
				nums[oi]=(ops[oi]=='+'?nums[oi]+nums[oi+1]:nums[oi]-nums[oi+1]);
				opDone=true;
				}

			if (opDone)
				{
				nums.erase(nums.begin()+oi+1);
				ops.erase(ops.begin()+oi);
				oi--;
				}
			}
		}

	return nums[0];
	}


std::string RadExp::toString()
	{
	std::string outStr="\n";
	outStr+="Variables: ";
	for (int i=0; i<this->variables.size(); ++i) { outStr+=this->variables[i]+((i<this->variables.size()-1)?", ":""); }
	outStr+="\n";
	outStr+="Constants: ";
	for (int i=0; i<this->constants.size(); ++i) { outStr+=std::to_string(this->constants[i])+((i<this->constants.size()-1)?", ":""); }
	outStr+="\n";
	outStr+="Operators: ";
	for (int i=0; i<this->operators.size(); ++i) { outStr+=std::string(1,this->operators[i])+((i<this->operators.size()-1)?", ":""); }
	outStr+="\n";
	outStr+="Pieces: ";
	for (int i=0; i<this->pieces.size(); ++i) { outStr+=(this->pieces[i]).toString()+((i<this->pieces.size()-1)?", ":""); }
	outStr+="\n";
	outStr+="\n";
	return outStr;
	}


#endif // RADEXP_CPP
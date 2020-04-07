
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
{
	r = vector <int> (REG_FILE_SIZE);
	mem = vector <int> (MEM_SIZE);
	limit = 0;
	halt = false;
	pc = 0;
	ir = 0;
	sp = MEM_SIZE;
}

int VirtualMachine::getClock()
{
	return _clock;
}

//Load and write the ops into mem.
bool VirtualMachine::loadIntoMem(fstream& obj)
{
	//If the file opened and we can read if no return false
	//Then load info from file into memory
	if (obj.is_open())
	{
		//Check to make sure we have mem to hold everything.
		//If not return false else continue loading into memory.
		while (!obj.eof())
		{
			if (limit >= mem.size())
			{
				return false;
			}
			else
			{
				obj >> mem[limit];
				limit++;
			}
		}

		return true;
	}

	return false;
}

//Run the instruction and then execute the OP in the execute function
void VirtualMachine::run(fstream& obj, fstream& inFile, fstream& outFile)
{
	//Load ObjFile into memory
	bool loaded = loadIntoMem(obj);

	if (loaded) {
		while (!halt)
		{
			ir = mem[pc];
			pc++;
			
			cout <<
			"clock: " << getClock << endl <<
			"register 0: " this->r[0] << endl <<
			"register 0: " this->r[0] << endl <<
			"register 0: " this->r[0] << endl <<
			"register 0: " this->r[0] << endl;
			
			executeOp(inFile, outFile);
		}
	}
	else
		cout << "Failed to load objFile into memory!" << endl;
}

//Execute the instruction
void VirtualMachine::executeOp(fstream& inFile, fstream& outFile)
{
	//Make into a struct? Would have it saved in private of class so could be used without having to pass it around
	//But then it takes up more memory cuz it is created at the start.
	bitset<5> op((ir & 0xf800) >> 11);
	bitset<2> rd((ir & 0x600) >> 9);
	bool immSet((ir & 0x100) >> 8);
	bitset<2> rs((ir & 0xc0) >> 6);
	bitset<8> addr(ir & 0xff);
	int constant(ir & 0xff);

	if (ir & 0x80)
		constant |= 0xffffff00;


	//DeBUGINNG
	cout << "OP CODE: " << op << endl;
	cout << "RD: " << rd << endl;
	cout << "IMMSET: " << immSet << endl;
	cout << "RS: " << rs << endl;
	cout << "ADDR: " << addr << endl;
	cout << "CONSTANT: " << constant << endl;
	cout << "PC: " << pc << endl;

	_clock++;

	//totally temp remove after figure it out
	switch (op.to_ulong())
	{
	case 0: //For load & loadi
		cout << "Load" << endl;
		if (immSet)
		{
			r[rd.to_ulong()] = constant;
		}
		else
		{
			r[rd.to_ulong()] = mem[addr.to_ulong()];
			_clock += 3;
		}
		break;

	case 1: //Store
		cout << "Store" << endl;
		mem[addr.to_ulong()] = r[rd.to_ulong()];
		_clock += 3;
		break;

	case 2: //Add & Addi & Carry is set
		cout << "Add" << endl;
		if (immSet)
		{
			r[rd.to_ulong()] += constant;
		}
		else
		{
			r[rd.to_ulong()] += r[rs.to_ulong()];
			//_clock += 3;
		}

		setCarry(rd);
		signExtend(rd);
		break;

	case 3: //addc & addci & Carry is set
		cout << "Addc" << endl;
		if (immSet)
			r[rd.to_ulong()] += constant & 0xffff;
		else {
			r[rd.to_ulong()] += r[rs.to_ulong()] & 0xffff;
			//_clock += 3;
		}

		setCarry(rd);
		signExtend(rd);
		break;

	case 4: //sub & subi & Carry is set
		cout << "Sub" << endl;
		if (immSet)
			r[rd.to_ulong()] -= constant;
		else
			r[rd.to_ulong()] -= r[rs.to_ulong()];

		setCarry(rd);
		signExtend(rd);
		break;

	case 5: //subc & subci & Carry is set
		cout << "Subc" << endl;
		if (immSet)
			r[rd.to_ulong()] -= constant;
		else
			r[rd.to_ulong()] -= r[rs.to_ulong()] & 0xffff;

		setCarry(rd);
		signExtend(rd);
		break;

	case 6: //and & andi
		cout << "And" << endl;
		if (immSet)
			r[rd.to_ulong()] &= constant;
		else
			r[rd.to_ulong()] &= r[rs.to_ulong()];
		break;
<<<<<<< Updated upstream

	case 7://xor & xori
		cout << "XOR" << endl;
		if (immSet) //xori r[RD] = r[RD] ^ CONST
=======
	case 7: //xor and xori
		if (immSet)
>>>>>>> Stashed changes
			r[rd.to_ulong()] = r[rs.to_ulong()] ^ constant;
		else//xor r[RD] = r[RD] ^ r[RS]
			r[rd.to_ulong()] ^= r[rs.to_ulong()];
		break;
<<<<<<< Updated upstream

	case 8://compl - r[RD] = ~ r[RD]
		cout << "Compl" << endl;
		r[rd.to_ulong()] = ~r[rd.to_ulong()];
		break;

	case 9: //shl - r[RD] = r[RD] << 1, shift-in-bit = 0
		cout << "SHL" << endl;
=======
		
	case 8: //compl
		r[rd.to_ulong()] = ~r[rd.to_ulong()];
		break;
		
	case 9: //shift-left
		r[rd.to_ulong()] = r[rd.to_ulong()] << 1;
>>>>>>> Stashed changes
		setCarry(rd);
		r[rd.to_ulong()] = r[rd.to_ulong()] << 1;
		break;

	case 10: //shla RD 
		cout << "shla" << endl;		
		r[rd.to_ulong()] <<= 1;		
		setCarry(rd);
		signExtend(rd);
		break;

	case 11: //shr RD - r[RD] = r[RD] >> 1, shift-in-bit = 0, set CARRY
		cout << "SHR" << endl;		

		if (r[rd.to_ulong()] & 0x01)
			sr |= 0x01;

		//If Value is neg then set sign extend
		if (r[rd.to_ulong()] & 0x8000) {
			r[rd.to_ulong()] = r[rd.to_ulong()] | 0xffff8000;
			r[rd.to_ulong()] = r[rd.to_ulong()] >> 1;
		}
		else //If lsb is not 1 then shift right no carry needed.
			r[rd.to_ulong()] = r[rd.to_ulong()] >> 1;

		break;

	case 12: //shra RD - shr arithmetic, Set CARRY & Sign Extend
		cout << "SHRA" << endl;
		if (r[rd.to_ulong()] & 0x01)
			sr |= 0x01;

		//If Value is neg then set sign extend
		if (r[rd.to_ulong()] & 0x8000) {
			r[rd.to_ulong()] = r[rd.to_ulong()] | 0xffff8000;
			r[rd.to_ulong()] = r[rd.to_ulong()] >> 1;
		}
		else //RD is pos just shift right dont need to sign extend
			r[rd.to_ulong()] = r[rd.to_ulong()] >> 1;
		break;

	case 13: //compr & compri
		cout << "COMPR" << endl;
		/*  if r[RD] < CONST set LESS reset EQUAL and GREATER;
			if r[RD] == CONST set EQUAL reset LESS and GREATER;
			if r[RD] > CONST set GREATER reset EQUAL and LESS; */
		if (immSet)
		{
			if (r[rd.to_ulong()] < constant) {
				//if (sr & 0x0008)
				//	sr |= 0x0008;
				//else
				//	sr &= ~0x0008;
				//Reset Less, Eq and Greater bits in SR.
				sr &= 0x0011;
				//Set less bit to 1
				sr |= 0x0008;
			}

			else if (r[rd.to_ulong()] == constant) {
				//if (sr & 0x0004)
				//	sr |= 0x0004;
				//else
				//	sr &= ~0x0004;

				//Set Eq bit to 1
				sr |= 0x0004;
			}

			else if (r[rd.to_ulong()] > constant) {
				//if (sr & 0x0002)
				//	sr |= 0x0002;
				//else
				//	sr &= ~0x0002;
				
				//Reset Less, EQ, Greater
				sr &= 0x0011;
				//Set greater bit to 1
				sr |= 0x0002;
			}
		}

		/*	if r[RD] < r[RS] set LESS reset EQUAL and GREATER;
			if r[RD] == r[RS] set EQUAL reset LESS and GREATER;
			if r[RD] > r[RS] set GREATER reset EQUAL and LESS; */
		else
		{
			if (r[rd.to_ulong()] < r[rs.to_ulong()]) {
				//if (sr & 0x0008)
				//	sr |= 0x0008;
				//else
				//	sr &= ~0x0008;
				
				//Reset Less, EQ, Greater in SR
				sr &= 0x0011;
				//Set less bit to 1
				sr |= 0x0008;
			}

			else if (r[rd.to_ulong()] == r[rs.to_ulong()]) {
				//if (sr & 0x0004)
				//	sr |= 0x0004;
				//else
				//	sr &= ~0x0004;

				//Reset Less, EQ, Greater in SR
				sr &= 0x0011;
				//Set less bit to 1
				sr |= 0x0004;
			}

			else if (r[rd.to_ulong()] > r[rs.to_ulong()]) {
				//if (sr & 0x0002)
				//	sr |= 0x0002;
				//else
				//	sr &= ~0x0002;
				//Reset Less, EQ, Greater
				sr &= 0x0011;
				//Set greater than bit to 1
				sr |= 0x0002;
			}
		}
		break;

	case 14: //getstat RD
		cout << "GETSTAT" << endl;
		r[rd.to_ulong()] = sr.to_ulong();
		break;

	case 15: //putstat RD
		cout << "PUTSTAT" << endl;
		sr = r[rd.to_ulong()];
		break;

	case 16: //jump ADDR
		cout << "JUMP" << endl;
		pc = addr.to_ulong();
		break;

	case 17: //jumpl ADDR
		cout << "JUMPL" << endl;
		if (sr.to_ulong() & 0x0008)
			pc = addr.to_ulong();
		break;

	case 18: //jumpe ADDR
		cout << "JUMPE" << endl;
		if (sr.to_ulong() & 0x0004)
			pc = addr.to_ulong();
		break;

	case 19: //jumpg ADDR
		cout << "JUMPG" << endl;
		if (sr.to_ulong() & 0x0002)
			pc = addr.to_ulong();
		break;

	case 20: //call ADDR - push VM status; pc = ADDR
		cout << "CALL" << endl;
		if (sp < limit + 6)
		{
			halt = true;
			return;
		}

		--sp;
		mem[sp] = pc;

		--sp;
		mem[sp] = r[0];

		--sp;
		mem[sp] = r[1];

		--sp;
		mem[sp] = r[2];

		--sp;
		mem[sp] = r[3];

		--sp;
		mem[sp] = sr.to_ulong();
		pc = addr.to_ulong();

		_clock += 3;
		
		break;

	case 21: //return - pop and return VM status
		cout << "RETURN" << endl;
		if (sp >= 256) {
			halt = true;
			return;
		}

		sr = mem[sp];
		++sp;

		r[3] = mem[sp];
		++sp;

		r[2] = mem[sp];
		++sp;

		r[1] = mem[sp];
		++sp;

		r[0] = mem[sp];
		++sp;

		pc = mem[sp];
		++sp;

		_clock += 3;
		break;

	case 22: // read RD - read new content r[RD] from .in file
		cout << "READ" << endl;
		inFile >> r[rd.to_ulong()];
		_clock += 27;
		break;

	case 23: // write RD - 	write r[RD] into .out file
		cout << "WRITE" << endl;
		outFile << r[rd.to_ulong()] << endl;
		_clock += 27;
		break;

	case 24: // halt - halt execution
		cout << "HALT" << endl;
		halt = true;
		break;

	case 25: // noop - 	no operation
<<<<<<< Updated upstream
		cout << "NOOP" << endl;
=======
		//FIXME: supposed to temporarily store a value?
>>>>>>> Stashed changes
		break;

	default:
		cout << "Bad opcode: " << ir << endl;
		break;
	}

	cout << "COUNTER: " << counter << endl;
	counter++;
	
	cout << endl;
}

void VirtualMachine::setCarry(bitset<2>& rd)
{
	if (r[rd.to_ulong()] & 0x10000)
		sr |= 01;
	else
		sr &= 0xe;
}

void VirtualMachine::setCarryR(bitset<2>& rd)
{
	if (r[rd.to_ulong()] & 0x01)
		sr |= 0x0001;
	else
		sr &= ~0x0001;
}

void VirtualMachine::signExtend(bitset<2>& rd)
{
	if(r[rd.to_ulong()] & 0x8000)
		r[rd.to_ulong()] |= 0xffff0000;
	else
		r[rd.to_ulong()] &= 0xffff;
}

#pragma once

class WriterZlib {
public:
	WriterZlib();
	~WriterZlib();

	bool init();
protected:
private:
	int def( FILE* source, FILE* target, int compLvl );
};
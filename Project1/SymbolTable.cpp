 #include "SymbolTable.h"
#include "Tokens.h"
 


SymbolTable::SymbolTable()
 {
	nextOffset=0;
	activationRecordSize=0;
	
 }
 
 
SymbolTable::~SymbolTable(void)
{
 	tables.clear();
 }
 
int SymbolTable::getNextOffset()
{
	int tempOffset=nextOffset;
	nextOffset++;
	return tempOffset;

}
 
 // Insert a record into the top most table in the vector

bool SymbolTable::insertRecord(string name, SymbolTable::Kind kind, Token token)
 {
 	Record* r;
 

	switch (kind)
 	{

	case SymbolTable::KIND_PROCEDURE:
	case SymbolTable::KIND_FUNCTION:
 		// add the function to the main symbol table 0

		r = lookupRecord(name,kind);
 
 		if (r == NULL)
 		{
 			r = new Record();
 			Table* t = tables.at(0);

			//r->col = col;
			//r->line = row;
			
 			r->name = name;
 			r->token = token;

			r->kind = kind;
			//r->offset=nextOffset; nextOffset++;
 			t->records.push_back(r);
 		}
 		// do not put break here

	case SymbolTable::KIND_VARIABLE:
		r = lookupRecord(name, kind);
 
 		if (r == NULL){
 			Table* t = tables.back();
 			r = new Record();

 			r->name = name;
 			r->token = token;

			r->kind = kind;
			r->offset= getNextOffset();
 			t->records.push_back(r);
			
			activationRecordSize++;
			return true;
 		}
 		break;
 	default:
		 	return true;
 }

}

bool SymbolTable::contains(string s, Kind k)
 {

	if (lookupRecord(s, k) == NULL) return false; else return true;
 }
 
 // Search the top most table in the vector, then its parent, and so on to the root table

SymbolTable::Record* SymbolTable::lookupRecord(string name, Kind kind,int table)
 {
 	// if a table number is specified then search that table, otherwise just search the top table
 	if (table > -1 && table < tables.size())

 		// search the table in reverse (start at the end of table)
 		for (int j = t->records.size() - 1; j >= 0; j--)
 		{

			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) /*&& (t->records.at(j)->kind == kind)*/ )
 				return t->records.at(j);
 		}
 	}
@@ -83,7 +99,7 @@ SymbolTable::Record* SymbolTable::lookupRecord(string name, Type type, int table
 
 		for (int j = t->records.size() - 1; j >= 0; j--)
 		{
-			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) && (t->records.at(j)->type == type) )
+			if ((strcmp(t->records.at(j)->name.c_str(), name.c_str()) == 0 ) /*&& (t->records.at(j)->kind == kind)*/ )
 				return t->records.at(j);
 		}
 	}
@@ -122,7 +138,7 @@ bool SymbolTable::createTable()
 	return true;
 }
 
-// Always remove the top most table in the vecotr
+// Always remove the top most table in the vector
 bool SymbolTable::removeTable()
 {
 	tables.pop_back();
int SymbolTable::tableSize(int t)
 	{
 		return tables.back()->records.size();
 	}


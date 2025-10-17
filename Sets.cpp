/**
 * 
 * CS 2430: Discrete Structures
 * Section 501
 * 
 * Author: Bryson Crader
 * Date: October 16th, 2025
*/
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <stdint.h>

/**
 * A Class Representing Bits As A String With N Bits Per String
 * 
 * BitString<N> Where N Is 2, For Example Could Be Any Of The Following:
 * 00, 01, 11, 10
 * 
 * BitStrings Are Chars Under The Hood, This Means An Ascii Char Can Be Passed To The
 * BitString, This Should Not Be Done, But It Is On The User To Not Send Chars Into A BitString
 */
template<size_t N = 3>
class BitString {
	private:
		/**
		 * The Actual Representation Of The BitString: An Array Of Chars
		 * 
		 * In Theory This Could Be An Integer And Be Printed In Binary Representation,
		 * But I Wanted The "String" Part Of Bit String, Plus Its Fun This Way
		*/
		char str[N];
	public:
		/**
		 * This Takes A Char Array Of Size N, And Sets The Internal BitString To It
		 */
		BitString(const char bits[N]) {
			for (int i = 0; i < N; i++)
				str[i] = bits[i];
		}

		/**
		 * This Takes A String And Sets The BitSet To The First N Bytes, All Bytes After N Are Ignored
		 * The String Must Be At Least N Bits
		 */
		BitString(const std::string& bits) {
			for (int i = 0; i < N; i++)
				str[i] = bits[i];
		}

		/**
		 * An Initialization With 0 Element Should Be Treated As A {0,0,...,0} Bits String With N Elements
		 */
		BitString() {
			for (int i = 0; i < N; i++)
				str[i] = '0';
		}

		/**
		 * This Will Convert The BitString Into A Normal String To Be Displayed In The Standard Output
		*/
		std::string toString() const {
			std::string string;
			for (int i = 0; i < N; i++)
				string += str[i];
			return string;
		}
		/** 
		 * This Allows The User To Use The "toString" Functionality By Casting The BitString To A std::string Type
		 * "this->toString" Should Behave The Same As "(std::string)this"
		*/
		operator std::string() const {
			return this->toString();
		}


		/** 
		 * Operator Allowing For Indexing Into The BitString.
		 * This Is Not Bounds Checked And May Throw Errors
		*/
		char& operator[](size_t at) {
			return str[at];
		}

		/**
		 * The Not Operator For BitStrings, Functions Much Like Boolean Not
		 * i.e. !true == false And !false == true
		 * 
		 * If We Are To Imagine A BitString As A Boolean Array: "001" [false,false,true]
		 * Then The Not Operator Would Be Applied To All Elements And Change To "110",
		 * Or In The Boolean Representation: [true,false,false]
		 * 
		 * This Operation Will Not Alter The Original BitString, Instead Returning A New One
		 * (See: Set::complement For Explination Of Post Argument "const")
		 */
		BitString<N> operator!() const {
			BitString<N> newBitStr{};
			for (int i = 0; i < N; i++) 
				newBitStr[i] = (str[i] == '0' ? '1' : '0');
			return newBitStr;
		}
		/**
		 * This Operator Will Check The Equivalence Of This BitString To Another BitString "strB".
		 * It Will Simply Check 1 By 1 If The Chars In The Strings Are The Same. 
		 * 
		 * The Template "N" (Representing Amounts Of Bits) Should Prevent Different Sized BitStrings
		 * From Being Compared, So There Is No Need To Check Size
		 * 
		 * Will Not Modify The Original BitString
		 */
		bool operator==(const BitString<N>& strB) const {
			for (int i = 0; i < N; i++)
				if (str[i] != strB.str[i]) return false;
			return true;
		}

		BitString<N>& operator=(const BitString<N>& strB) {
			for (int i = 0; i < N; i++)
				str[i] = ((BitString<N>)strB)[i];
			return *this;
		}

		/**
		 * This Will Generate The Power Set Of N Bits:
		 * For Example, If N Is 3 The Power Set Will Be:
		 * {000, 001, 010, 011, 100, 101, 111}
		 * 
		 * Conicidentally, The Power Set Of BitStrings Is The Equivalent Of
		 * The Integers 0 to (2^N)-1, Which Is Also Equivalent To Adding 1 To
		 * An Integer From 0 Until The Max Value Of An N-Bit Integer
		 */
		static std::vector<BitString<N>> GeneratePowerSet() {
			std::vector<BitString<N>> bitStrings(pow(2, N));
			for (int i = 0; i < pow(2, N); i++)
				bitStrings[i] = std::bitset<N>(i).to_string().c_str();
			return bitStrings;
		}
};

/**
 * Helper Function To Find Indices:
 * Returns -1 If Not Found, Otherwise Returns The Index Of The Found Element
 */
template<typename Find_T>
int GetIndexOfElement(const std::vector<Find_T>& vec, const Find_T item) {
	int it = 0;
	for (const auto& i : vec) {
		if (i == item) return it;
		it++;
	}
	return -1;
}


/**
 * Each Set is represented as an array of values with possible operations to be preformed on them
 */
template<size_t N_Bits>
class Set {
	BitString<N_Bits>* elements;
	size_t size;
	
	public:
		/**
		 * Set Constructor With Manual Element Creation:
		 * 		Creates A Set With Elements {z_1, z_2, ..., z_N}
		 */
		Set(std::initializer_list<BitString<N_Bits>> initElements) {
			// allocate memory for the elements
			elements = (BitString<N_Bits>*)malloc(sizeof(BitString<N_Bits>) * initElements.size());
			size = initElements.size();
		
			// copy elements from parameter to set
			int i = 0;
			for (const auto& elm : initElements)
				elements[i++] = elm;
		};
		/**
		 * Set Constructor With Size Creation:
		 * 		Creates A Set With No Elements Set, But Space For N Elements Of Type T To Be Put In The Set
		 */
		Set(size_t N) {
			elements = (BitString<N_Bits>*)malloc(sizeof(BitString<N_Bits>) * N);
			size = N;
		}
		/**
		 * Set Deconstructor To Call "free" On Heap Allocated Memory Created In Constructors
		 * This May Or May Not Be Needed Depending On Operating System, Often, At Least On Windows,
		 * The Operating System Cleans Up Loose Memory Once The Program Stop
		 */
		~Set() {
			free(elements);
		}

		/**
		 * The [] Operator Allows The User To Index Into The Set
		 * This Operations Is 0-Indexed
		 * 
		 * This Operations Can Throw Errors, It Is Not Bounds Checked
		*/
		BitString<N_Bits>& operator[](size_t at) {
			return elements[at];
		}

		/**
		 * The = Operator Will Copy The Elements From One Set ("setB") And Put Them In This Set ("setA"/this)
		*/
		Set<N_Bits>& operator=(const Set<N_Bits>& setB) {
			this = Set<N_Bits>(setB.size);
			for (int i = 0; i < setB.size; i++) {
				elements[i] = setB[i];
			}
			return *this;
		}


	 	/**
		 * NOT(A) (Cant Be Called "not" As That Is A Keyword)
		 * 
		 * The Not/Complement Operation Gives A Set Of Every Element Not Inside Set A
		 * 
		 * This Operation Will Not Modify The Original Set, Therefore, "const" Is Used To 
		 * Tell The Compiler That The Method Is Not Going To Modify Itself. This Returns A 
		 * New Set With The Operation Preformed On Each Element Of The Current Set:
		 * 
		 * NewSetA = NOT(A)
		 */
		Set<N_Bits> complement() const {
			// Get The Power Set Of N-Bits
			std::vector<BitString<N_Bits>> powerSet = BitString<N_Bits>::GeneratePowerSet();
			
			// All Elements Of This Set Should Exist Within The Power Set,
			// We Just Need To Remove Those Elements That Exist Within This
			// Set And Return The Remaining Elements Of The Power Set
			for (int i = 0; i < size; i++) {
				// The Position/Index Of The Set's Currently Iterated Element
				int pos = GetIndexOfElement(powerSet, elements[i]);

				if (pos < 0) {
					fprintf(stderr, "No Matches Found\n");
				} else {
					powerSet.erase(powerSet.begin()+pos);
				}
			}

			// Copy The Remaining Power Set Over To The New Set "notSet"
			Set<N_Bits> notSet(powerSet.size());
			for (int i = 0; i < powerSet.size(); i++) {
				notSet[i] = powerSet[i];
			}
			return notSet;
		}
		
		/**
		 * A ∪ B == Union (Cant Be Called "union" As That Is A Keyword)
		 * 
		 * The Union Operation Gets the Elements Of Both Sets In A Single Set, This Also Avoids Copying Duplicates
		 * The Operation Will Not Modify The Original Set (See: Set::complement For Explination).
		 * 
		 * Each Individual Element Will Be Copied Into An Auxiliary Array, Not Copying Duplicates
		*/
		Set<N_Bits> unionOp(const Set<N_Bits>& setB) const {
			// Create A Hashmap With A String Key And BitString Value
			// This Allows For Removing Duplicates With Ease By Using
			// The Same Key Many Times, And Overwriting Old Keys Of The
			// Same Value
			std::unordered_map<std::string, BitString<N_Bits>> map;

			// Push All Elements From This Set Into A Hashmap
			for (int i = 0; i < this->size; i++)
				map[(std::string)elements[i]] = elements[i];
			
			// Push All Elements From Set B "setB" Into Same Hashmap,
			// If Elements Share A Key, The Old Key Will Be Overwritten,
			// Thereby Avoiding Duplicates
			for (int i = 0; i < setB.size; i++)
				map[(std::string)(setB.elements[i])] = setB.elements[i];

			// Create A New Set With All The Elements From The Hashmap, Which
			// Should Have No Duplicates, Which Means The Size Of The New Set Should
			// Equal The Size Of The Hashmap
			Set<N_Bits> newSet(map.size());

			// Copy From Map To New Set
			int iter = 0;
			for (const auto& elm : map) {
				newSet[iter++] = elm.second;
		 	}

			return newSet;
		}

		/**
		 *  A ∩ B == Intersection
		 * 
		 * The Intersection Operation Gives A Set Of Elements That Are In Both Set A And Set B
		 * 
		 * Will Not Modify The Original Set, Will Return New Set
		*/
		Set<N_Bits> intersection(const Set<N_Bits>& setB) const {
			// Use Another Hashmap With The BitString As The Key, And Number Of Occurances As The Value:
			// map[bitstring] Will Return: (int) occurances
			// If Occurances > 1 -- Meaning In Both Sets, Add It To The New Set

			std::unordered_map<std::string, int> map;

			// Set All Elements To 1, All Elements Of Set A Should Exist
			for (int i = 0; i < this->size; i++) {
				auto key = (std::string)elements[i];
				map[key] = 1;
			}
				
			// If The BitString (Key) Exists (Which Should Equal 1) And 
			// Set B Contains The Element, Add 1
			// This Should Not Add Any New Elements To The Map,
			// Which Means Set C (The Set Being Returned) Will Have A Max
			// Size Of "map.size()"
			for (int i = 0; i < setB.size; i++) {
				auto key = (std::string)(setB.elements[i]);
				if (map.contains(key)) map[key]++;
			}

			// If Any map[bitstring] > 1 Then It Exists In Both,
			// So We May Copy It Over To A New Dynamic Array (Vector),
			// We Cant Copy It Directly Over Because Any Element In
			// The Array May Be Equal To 1, Meaning It Will Not Be
			// Part Of The Final Result, So "map.size()" Wont Equal
			// Set C ("newSet") Size

			std::vector<BitString<N_Bits>> intersections{};
			for (const auto& elm : map) {
				// check map[bitstring].occurances > 1
				if (elm.second > 1)
					intersections.push_back(BitString<N_Bits>(elm.first));
			}

			// Set C/"newSet" Size Will Equal "intersections.size()"
			Set<N_Bits> newSet(intersections.size());
			for (int i = 0; i < intersections.size(); i++)
				newSet[i] = intersections[i];

			return newSet;
		}

		/**
		 * A - B == Difference
		 * 
		 * The Difference Operation Gives A Set That Contains All Elements That Are In A, But Not In B
		 */
		void difference() {}
		/**
		 * A ⊕ B == Symetric Difference
		 * 
		 * The Symetric Difference Operation Gives A Set That Has Elements Of Set A And Set B, But Not Set A and B
		 * XOR In Other Words
		 */
		void symetricDifference() {}

		/**
		 * Prints Out All Of The Elements Of The Set
		 */
		void print() const {
			std::cout << "[";
			for (size_t i = 0; i < this->size; i++) {
				std::cout << elements[i].toString() << (i<size-1 ? ", " : "");
			}
			std::cout << "]\n";
		}
};

class MultiSet {

};


/**
 * Runs A Given Function With Header And Footer Bars To Seperate Test,
 * This Also Allows New Scopes So Each Test Can Have A Set "A" and "B"
 */
void RunTest(std::string label, std::function<void()> func) {
	std::string title = "===== " + label + " =====";
	std::cout << title << "\n\n";
	func();
	std::cout << "\n" << std::string(title.size(),'=') << "\n";
}

auto main() -> int {

	RunTest("Test Init Sets", []() {
		Set<3> A {"000","001","010"};
		Set<3> B {"000","100","0101"};

		A.print();
		B.print();
	});


	RunTest("Test Set Complement", []() {
		printf("Set:      ");
		Set<3> A {"000"};
		A.print();
		
		printf("NOT(Set): ");
		Set<3> B = A.complement();
		B.print();
	});

	RunTest("Test Set Union", []() {
		printf("Sets:\n");
		printf("A:     ");
		Set<3> A {"000","001"};
		A.print();
		printf("B:     ");
		Set<3> B {"100","010"};
		B.print();

		printf("Union: ");
		Set<3> C = A.unionOp(B);
		C.print();
	});

	RunTest("Test Set Intersection", []() {
		printf("Sets:\n");

		Set<3> A {"000","010","100"};
		printf("A:            ");
		A.print();
		
		Set<3> B {"000","010","001"};
		printf("B:            ");
		B.print();
		
		Set<3> C = A.intersection(B);
		printf("Intersection: ");
		C.print();
	});

	return 0;
}
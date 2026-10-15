#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>


constexpr int max_bool_func_args {10};

using namespace boost::multiprecision;

using BoolTable = std::vector<std::vector<bool>>;

template <typename T = uint1024_t>
class Boolnum {
	public:
		Boolnum() : bit_count{-1}, num{0} {}

		Boolnum(T num) : bit_count{-1}, num{num} {}

		Boolnum(T num, int bit_count) : bit_count{bit_count}, num{num} {}

		Boolnum(const char* str) : bit_count{-1}, num{str} {}

		const T& getnum() const { return num; }

#if 1
		void setnum(T num, int bit_count) { this->num = num; this->bit_count = bit_count; }

		void setnum(T num) { this->num = num; this->bit_count = -1; }
#endif
	
		bool getbit(int index) const
		{
			check_index(index);
			T mask {1};
			return (num & (mask << index)) != 0;
		}
		
		void setbit(int index, bool bit)
		{
			check_index(index);	
			T mask {1};
			if(bit) {
				this->num = this->num | (mask << index);
			}
			else {
				this->num = this->num & ~(mask << index);

			}
		}
	
		void print() const { std::cout << num << std::endl; }
		
		void print_bits() const
		{
			check_bc_error();
			for(int i = 0; i < bit_count; ++i) {
				std::cout << getbit( (bit_count-1) - i );
			}
			std::cout << std::endl;
		}

		void print_bits(int bit_count) const
		{
			for(int i = 0; i < bit_count; ++i) {
				std::cout << getbit( (bit_count-1) - i );
			}
			std::cout << std::endl;
		}

		bool operator<(const Boolnum& other) { return num < other.getnum(); }
		bool operator>(const Boolnum& other) { return num > other.getnum(); }

		void check_bc_error() const
		{
			if(bit_count == -1) {
				throw std::runtime_error {"Boolnum bit_count should be not -1"};
			}
		}

		int bit_count;

	private:
		T num;

		static void check_index(int index)
		{
			if(index < 0 || index > std::numeric_limits<T>::digits - 1) {
				throw std::runtime_error{"Boolnum is out of bounds"};
			}

		}
};

template<typename T>
T pow2(int pow)
{
	T num {1};
	return num << pow;
}

int log2(int num)
{
	int i {num};
	int res {0};
	for (; ; ++res) {
		i = i >> 1;
		if(i == 0) {break;}
	}
	if(num != pow2<int>(res)) {
		throw std::runtime_error {"log2 func arg should be power of 2"};
	}
	return res;
}

template<typename T>
std::vector<T> get_pow2_vector(int max_pow)
{
	std::vector<T> pows;
	for(int i = 0; i <= max_pow; ++i) {
		pows.push_back(pow2<T>(i));
	}
	return pows;
}

template<typename T>
void print_container(const T& c)
{
	for(const auto& val : c) {
		std::cout << val << std::endl;
	}
}

template<typename T>
void print_container2(const T& c)
{
	for(const auto& row : c) {
		for(const auto& val : row) {
			std::cout << val << "\t";
		}
		std::cout << std::endl;
	}
}

template<typename T = uint1024_t>
class Truth_table {
	public:
		Truth_table() : result_vector{0}, args_count{0}, rows_count{0} {}

		Truth_table(int args_count, const Boolnum<T>& result_vector)
		{
			generate(args_count, result_vector);
		}

		void generate(int args_count, const Boolnum<T>& result_vector)
		{
			rows_count = pow2<int>(args_count);
			auto max_result_vector {pow2<T>(rows_count) - 1};
			if(result_vector.getnum() > max_result_vector) {
				throw std::runtime_error{"the value of truth table result vector is bigger than it can be"};	
			}
			if(args_count > max_bool_func_args) {
				throw std::runtime_error{"bool func has too much args"};
			}
			this->result_vector.setnum(result_vector.getnum(), rows_count);
			this->args_count = args_count;
		}

		void print()
		{
			std::cout << "result vector:\t";
			result_vector.print_bits();
			std::cout << "args count:\t" << args_count << std::endl << "rows count:\t" << rows_count << std::endl;
		}

		Boolnum<T> result_vector;
		int args_count;
		int rows_count;
};

template<typename T>
Boolnum<T> combine_Boolnums(const Boolnum<T>& msb, const Boolnum<T>& lsb)
{
	msb.check_bc_error();
	lsb.check_bc_error();
	Boolnum<T> res {(msb.getnum() << lsb.bit_count) + lsb.getnum()};
	res.bit_count = msb.bit_count + lsb.bit_count;
	return res;

}

struct row_column {
	int row;
	int column;
	};


template<typename T = uint1024_t>
class K_map {
	//result vector LSB is 0 of truth table

	public:
		K_map() {}

		K_map(Truth_table<T> truth_table)
		{
			make(truth_table);
			generate_conjunctions();
		}

		K_map(int func_args_count, const BoolTable& data)
		{
			make_direct(func_args_count, data);
			generate_conjunctions();
		}

		void print(int conjunction_index = -1)
		{
			if(conjunction_index == -1) {
				uni_print_(data, "1", "0");
			}
			else {
				uni_print_(conjunctions[conjunction_index], "+", ".");
			}
		}

		void print_conjunctions()
		{
			int conjunction_index {0};
			for( [[maybe_unused]]const auto& conjunction : conjunctions) {
				std::cout << "conjuncion " << conjunction_index << std::endl; 
				print(conjunction_index);
				++conjunction_index;
			}
		}

		std::vector<Boolnum<T>> make_gray_code(int bit_count)
		{
			std::vector<Boolnum<T>> codes;
			int codes_count {pow2<int>(bit_count)};	
			for(T i = 0; i < codes_count; ++i) {
				T temp {i ^ (i >> 1)}; // ^ is XOR
				codes.push_back({temp, bit_count});
			}
			return codes;
		}

		std::array<int, 2> split_to_halfs(int num)
		{
			if(num<2) {
				throw std::runtime_error {"the num for splitting by halfs is < 2"};
			}
			int lower {num/2};
			int higher {num - lower};
			return {higher, lower};
		}

		void make_head(int func_args_count)
		{
			auto [row_args_count, column_args_count] {split_to_halfs(func_args_count)};
			gray_code_row = make_gray_code(row_args_count);
			gray_code_column = make_gray_code(column_args_count);

			max_size.row = static_cast<int>(gray_code_row.size());
			max_size.column = static_cast<int>(gray_code_column.size());
		}

	
		void generate_conjuction_sizes()
		{
			int max_area_size = max_size.row * max_size.column;
			std::vector<int> conjuctions_area_sizes = get_pow2_vector<int>(log2(max_area_size));

			for(int area_size_index = conjuctions_area_sizes.size() - 1; area_size_index >= 0 ; --area_size_index) { 
				for(int columnsize {1}, rowsize {1}; ; rowsize = rowsize << 1) {
					columnsize = conjuctions_area_sizes[area_size_index] / rowsize;
					if(columnsize < 1) {break;}
					if(columnsize <= max_size.column && rowsize <= max_size.row) {
					conjunctions_sizes.push_back({rowsize, columnsize});		
					}
				}
			}
		}

		void make(Truth_table<T> truth_table)
		{
			make_head(truth_table.args_count);

			for(size_t row = 0; row < gray_code_column.size(); ++row) {
				data.push_back({});
				for(size_t column = 0; column < gray_code_row.size(); ++column) {
					Boolnum<T> combined_gray_code = combine_Boolnums(gray_code_column[row], gray_code_row[column]);
					auto index = combined_gray_code.getnum();
					bool bit = truth_table.result_vector.getbit(static_cast<int>(index)); 
					data[row].push_back(bit);
				}
			}

			generate_conjuction_sizes();
		}

		void make_direct(int func_args_count, const BoolTable& data)
		{
			make_head(func_args_count);
			if( (gray_code_column.size() != data.size()) || (gray_code_row.size() != data[0].size()) ) {
				throw std::runtime_error {"in make_direct func, count of func args not matched with given data"};
			}
			this->data = data;
			generate_conjuction_sizes();
		}

		void fill_zeros(std::vector<std::vector<bool>>& conjunction)
		{
			for(int row = 0; row < max_size.column; ++row) {
				conjunction.push_back({});
				for(int column = 0; column < max_size.row; ++column) {
					conjunction[row].push_back(false);
				}
			}
		}

		enum class Dir { 
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		void step(row_column& point, Dir dir)
		{
			switch(dir) {
				case Dir::UP:
					if(point.row == 0) {
						point.row = max_size.column - 1;
					}
					else { --point.row; }
					break;
				case Dir::DOWN:
					if(point.row == (max_size.column - 1)) {
						point.row = 0;
					}
					else { ++point.row; }
					break;
				case Dir::LEFT:
					if(point.column == 0) {
						point.column = max_size.row - 1;
					}
					else { --point.column; }
					break;
				case Dir::RIGHT:
					if(point.column == (max_size.row - 1)) {
						point.column = 0;
					}
					else { ++point.column; }
					break;
			}
		}

		void generate_conjunction(row_column conjunction_corner_position, row_column conjunction_size)
		{
			auto& sz = conjunction_size;
			auto& cr = conjunction_corner_position;

			BoolTable conjunction;
			fill_zeros(conjunction);
			row_column point {cr.row, cr.column};

			for(int i = cr.row; i < (cr.row + sz.column); ++i) {
				int j {0};
				for(j = cr.column; j < (cr.column + sz.row); ++j) {
					conjunction[point.row][point.column] = true;
					step(point, Dir::RIGHT);
				}
				for(; j > cr.column; --j) { step(point, Dir::LEFT); }
				step(point, Dir::DOWN);
			}

			conjunctions.push_back(conjunction);
		}

		bool check_conjunction_availability(row_column conjunction_corner_position, row_column conjunction_size)
		{
			auto &sz = conjunction_size;
			auto &cr = conjunction_corner_position;
			
			row_column point {cr.row, cr.column};
			std::vector<int> cbc; //cbc is conjunction bit count
			for([[maybe_unused]] const auto& v : this->conjunctions) {
				cbc.push_back(0);
			}

			for(int i = cr.row; i < (cr.row + sz.column); ++i) {
				int j {0};
				for(j = cr.column; j < (cr.column + sz.row); ++j) {
					if(data[point.row][point.column] == 0) { return false; }
					for(size_t ci = 0; ci < this->conjunctions.size(); ++ci) { // ci is conjunction index
						if(conjunctions[ci][point.row][point.column] == 1) {
							++cbc[ci];
						}
					}
					step(point, Dir::RIGHT);
				}
				for(; j > cr.column; --j) { step(point, Dir::LEFT); }
				step(point, Dir::DOWN);
			}
			int conjunction_area = conjunction_size.row * conjunction_size.column;
			for(const auto& val : cbc) {
				if(val >= conjunction_area) { return false; }
			}
			return true;

		}

#if 1
		void generate_conjunctions()
		{
			for(const auto& conjunction_size : conjunctions_sizes) {
				for(int row = 0; row < max_size.column; ++row) {
					for(int column = 0; column < max_size.row; ++column) {
						row_column conjunction_corner_position = {row, column};
						if(check_conjunction_availability(conjunction_corner_position, conjunction_size)) {
							generate_conjunction(conjunction_corner_position, conjunction_size);
						}
					}
				}
			}
		}
#endif

		std::vector<row_column> conjunctions_sizes;

	private:
		void uni_print_(const BoolTable& container, const char* pos, const char* neg)
		{
			std::cout << std::endl;
			for(const auto& row : container) {
				for(const auto& bit : row) {
					std::cout << (bit ? pos : neg) << "   ";
				}
				std::cout << std::endl << std::endl;
			}
		}


		BoolTable data;

		std::vector<BoolTable> conjunctions;

		std::vector<Boolnum<T>> gray_code_row;
		std::vector<Boolnum<T>> gray_code_column;

		row_column max_size;

		friend class MakeMapHead_Simple_Test;
		friend class MakeMap_Simple_Test;
		friend class MapMakeDirect_Simple_Test;
		friend class MapMake_Visualise_Test;
		friend class GenerateConjuctionSizes_Simple_Test;
		friend class GenerateConjunction_Simple_Test;
		friend class GenerateConjunctions_FourByFour_Test;

};

std::ostream& operator<<(std::ostream& os, const row_column& rc)
{
	os << "row:\t" << rc.row << "\t column:\t" << rc.column;
	return os;
}


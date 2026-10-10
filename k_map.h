#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>


constexpr int max_bool_func_args {10};

using namespace boost::multiprecision;

template <typename T>
class Boolnum {
	public:
		Boolnum() {}

		Boolnum(T num) : num{num} {}

		const T& getnum() const { return num; }

		void setnum(T num) { this->num = num; }
	
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
		
		void print_bits(int bit_count) const
		{
			for(int i = 0; i < bit_count; ++i) {
				std::cout << getbit( (bit_count-1) - i );
			}
			std::cout << std::endl;
		}

		bool operator<(const Boolnum& other) { return num < other.getnum(); }
		bool operator>(const Boolnum& other) { return num > other.getnum(); }

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
class Truth_table {
	public:
		Truth_table() : result_vector{0}, args_count{0}, rows_count{0} {}

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
			this->result_vector.setnum(result_vector.getnum());
			this->args_count = args_count;
		}

		void print()
		{
			std::cout << "result vector:\t";
			result_vector.print_bits(rows_count);
			std::cout << "args count:\t" << args_count << std::endl << "rows count:\t" << rows_count << std::endl;
		}

		Boolnum<T> result_vector;
		int args_count;
		int rows_count;
};

template<typename T>
class K_map {
	//bool func MSB argument contains in column
	//result vector LSB is 0 of truth table

	public:
		void print()
		{
			std::cout << std::endl;
			for(const auto& row : map_data) {
				for(const auto& bit : row) {
					std::cout << bit << "   ";
				}
				std::cout << std::endl << std::endl;
			}
		}

		std::vector<Boolnum<T>> make_gray_code(int bit_count)
		{
			std::vector<Boolnum<T>> codes;
			int codes_count {pow2<int>(bit_count)};	
			for(T i = 0; i < codes_count; ++i) {
				T temp {i ^ (i >> 1)}; // ^ is XOR
				codes.push_back(temp);
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

		void make_map_head(int func_args_count)
		{
			auto [row_args_count, column_args_count] {split_to_halfs(func_args_count)};
			gray_code_row = make_gray_code(row_args_count);
			gray_code_column = make_gray_code(column_args_count);
		}

		Boolnum<T> combine_column_row_code(int column, int row)
		{
			//GOVNO
			Boolnum<T> code {0};
			int code_bit_num {0};
			for(const auto& bit : row) {
				code.setbit(code_bit_num, bit)
				++code_bit_num;
			}
			int code_bit_num {0}
			for(const auto& bit : column) {
				code.setbit(code_bit_num, bit)
				++code_bit_num;
			}
			return code;
		}
		
		void make_map(Truth_table<T> truth_table)
		{
			make_map_head(truth_table.args_count);

			for(size_t column = 0; column < gray_code_column.size(); ++column) {
				map_data.push_back({});
				for(size_t row = 0; row < gray_code_row.size(); ++row) {
					auto combined_code {combine_column_row_code(column, row)};
					int combined_code_int static_cast<int>combined_code.getnum();
					bool bit = truth_table.result_vector.getbit(combined_code_int);
					map_data[column].push_back(bit);
				}
			}
		}

	private:
		std::vector<std::vector<bool>> map_data;
		std::vector<Boolnum<T>> gray_code_row;
		std::vector<Boolnum<T>> gray_code_column;

		friend class MakeMapHead_Simple_Test;
};


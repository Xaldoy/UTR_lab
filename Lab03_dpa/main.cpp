#include <iostream>
#include <string>

std::string output;
size_t position;
std::string input;

bool check_S();
bool check_A();
bool check_B();
bool check_C();
void parse();

bool check_S()
{
    output = output.append("S");
    if (position < input.length() && input[position] == 'a')
    {
        position++;
        if (check_A() && check_B())
        {
            return true;
        }
    }
    else if (position < input.length() && input[position] == 'b')
    {
        position++;
        if (check_B() && check_A())
        {
            return true;
        }
    }
    return false;
}

bool check_A()
{
    output = output.append("A");
    if (position < input.length() && input[position] == 'b')
    {
        position++;
        if (check_C())
        {
            return true;
        }
    }
    else if (position < input.length() && input[position] == 'a')
    {
        position++;
        return true;
    }
    return false;
}

bool check_B()
{
    output = output.append("B");
    if (position + 1 < input.length() && input[position] == 'c' && input[position + 1] == 'c')
    {
        position += 2;
        if (check_S() && position < input.length() && input[position] == 'b' && position + 1 < input.length())
        {
            position += 2;
            return true;
        }
    }
    return true;
}

bool check_C()
{
    output = output.append("C");
    return check_A() && check_A();
}

void parse()
{
    if (check_S())
    {
        if (position == input.length())
        {
            std::cout << output << std::endl;
            std::cout << "DA" << std::endl;
            return;
        }
    }
    std::cout << output << std::endl;
    std::cout << "NE" << std::endl;
}

int main()
{
    std::getline(std::cin, input);
    position = 0;
    output = "";
    parse();
}

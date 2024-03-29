#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>

class DeltaTransition
{
public:
    std::string input, fromState, toState;
    DeltaTransition(const std::string &_input, const std::string &_fromState, const std::string &_toState) : input(_input), fromState(_fromState), toState(_toState) {}
};

class ENKA
{
private:
    std::vector<std::string> inputAlphabet;
    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::vector<std::string> acceptableStates;
    std::string initialState;
    std::vector<DeltaTransition> deltaTransitions;

    std::set<std::string> findEpsilonEnvironment(const std::string &state)
    {
        std::set<std::string> epsilonEnvironment;
        for (auto transition : deltaTransitions)
        {
            if (transition.fromState == state && transition.input == "$")
            {
                epsilonEnvironment.insert(transition.toState);
                std::set<std::string> temp = findEpsilonEnvironment(transition.toState);
                epsilonEnvironment.insert(temp.begin(), temp.end());
            }
        }
        return epsilonEnvironment;
    }

public:
    ENKA(const std::vector<std::string> &inputAlphabet_,
         const std::vector<std::string> &states_,
         const std::vector<std::string> &alphabet_,
         const std::vector<std::string> &acceptableStates_,
         const std::string &initialState_,
         const std::vector<DeltaTransition> &deltaTransitions_) : inputAlphabet(inputAlphabet_), states(states_), alphabet(alphabet_),
                                                                  acceptableStates(acceptableStates_), initialState(initialState_),
                                                                  deltaTransitions(deltaTransitions_)
    {
    }

    void simulate()
    {
        for (auto eps : findEpsilonEnvironment(initialState))
        {
            std::cout << eps << std::endl;
        }
    }
};

std::vector<std::string> tokeniseComma(std::string &input)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ','))
        result.push_back(token);

    return result;
}

DeltaTransition createDeltaTransition(std::string &input)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;

    std::getline(ss, token, ',');
    result.push_back(token);
    std::getline(ss, token, '-');
    result.push_back(token);
    std::getline(ss, token);
    result.push_back(token.substr(1));

    DeltaTransition deltaTransition = DeltaTransition(result[0], result[1], result[2]);
    return deltaTransition;
}

int main()
{
    std::vector<std::string> inputAlphabet;
    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::vector<std::string> acceptableStates;
    std::string initialState;
    std::vector<std::string> transitionFunctions;
    std::string input;

    std::getline(std::cin, input);
    inputAlphabet = tokeniseComma(input);
    std::getline(std::cin, input);
    states = tokeniseComma(input);
    std::getline(std::cin, input);
    alphabet = tokeniseComma(input);
    std::getline(std::cin, input);
    acceptableStates = tokeniseComma(input);
    std::getline(std::cin, input);
    initialState = input;

    std::string line;
    while (std::getline(std::cin, line))
    {
        transitionFunctions.push_back(line);
    }

    std::vector<DeltaTransition> deltaTransitions;

    for (auto &transition : transitionFunctions)
    {
        deltaTransitions.push_back(createDeltaTransition(transition));
    }

    ENKA enka(inputAlphabet, states, alphabet, acceptableStates, initialState, deltaTransitions);

    enka.simulate();

    return 0;
}

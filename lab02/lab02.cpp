#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <iterator>

class DeltaTransition
{
public:
    std::string input, fromState, toState;
    DeltaTransition(const std::string &_input, const std::string &_fromState, const std::string _toState) : input(_input), fromState(_fromState), toState(_toState) {}
    bool operator==(const DeltaTransition &other) const
    {
        return (input == other.input) && (fromState == other.fromState) && (toState == other.toState);
    }
};

std::set<std::string> subtractSets(std::set<std::string> set1, std::set<std::string> set2)
{
    std::set<std::string> diff;
    std::set_difference(set1.begin(), set1.end(),
                        set2.begin(), set2.end(),
                        std::inserter(diff, diff.end()));
    return diff;
}

std::set<std::string> interceptSets(std::set<std::string> set1, std::set<std::string> set2)
{
    std::set<std::string> intersection;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}

class DKA
{
private:
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> acceptableStates;
    std::string initialState;
    std::vector<DeltaTransition> deltaTransitions;

    std::set<std::string> deltaTransition(const std::string &state, const std::string &input = "")
    {
        std::set<std::string> result;
        for (const auto &transition : deltaTransitions)
        {
            if (transition.fromState == state && (transition.input == input || input == ""))
            {
                result.insert(transition.toState);
            }
        }
        return result;
    }

public:
    DKA(
        const std::set<std::string> &states_,
        const std::set<std::string> &alphabet_,
        const std::set<std::string> &acceptableStates_,
        const std::string &initialState_,
        const std::vector<DeltaTransition> &deltaTransitions_) : states(states_), alphabet(alphabet_),
                                                                 acceptableStates(acceptableStates_), initialState(initialState_),
                                                                 deltaTransitions(deltaTransitions_)
    {
    }

    void removeRelated(bool removeTransitions = true)
    {
        if (removeTransitions)
        {
            std::vector<DeltaTransition> updatedTransitions = {};
            for (const auto &transition : deltaTransitions)
            {
                if (states.find(transition.fromState) != states.end() && states.find(transition.toState) != states.end())
                {
                    updatedTransitions.push_back(transition);
                }
            }
            deltaTransitions = updatedTransitions;
        }
        std::set<std::string> updatedAcceptible = {};
        for (const auto &state : acceptableStates)
        {
            if (states.find(state) != states.end() && states.find(state) != states.end())
            {
                updatedAcceptible.insert(state);
            }
        }
        acceptableStates = updatedAcceptible;
    }

    void removeUnreachable()
    {
        std::set<std::string> oldReachable;
        std::set<std::string> newReachable = {initialState};
        while (oldReachable != newReachable)
        {
            oldReachable = newReachable;
            std::set<std::string> temp;
            for (const auto &state : oldReachable)
            {
                std::set<std::string> tempState = deltaTransition(state);
                newReachable.insert(tempState.begin(), tempState.end());
            }
        }
        std::set<std::string> unreachableStates = subtractSets(states, newReachable);
        states = newReachable;
        removeRelated();
    }

    void minimize()
    {
        std::set<std::string> unnaceptableStates = subtractSets(states, acceptableStates);
        std::set<std::set<std::string>> P = {acceptableStates, unnaceptableStates};
        std::set<std::set<std::string>> W = {acceptableStates, unnaceptableStates};

        while (W.size() > 0)
        {
            std::set<std::string> A;
            auto it = W.begin();
            A = *it;
            W.erase(it);

            for (const auto &c : alphabet)
            {
                std::set<std::string> X;
                for (const auto &transition : deltaTransitions)
                {
                    if (transition.input == c && A.find(transition.toState) != A.end())
                    {
                        X.insert(transition.fromState);
                    }
                }

                std::set<std::set<std::string>> tempP = P;

                for (auto &Y : tempP)
                {
                    std::set<std::string> interception = interceptSets(Y, X);
                    std::set<std::string> subtraction = subtractSets(Y, X);
                    if (interception.size() == 0 || subtraction.size() == 0)
                        continue;
                    P.erase(P.find(Y));
                    P.insert(interception);
                    P.insert(subtraction);

                    std::set<std::set<std::string>> tempW = W;

                    if (tempW.find(Y) != tempW.end())
                    {
                        W.erase(W.find(Y));
                        W.insert(interception);
                        W.insert(subtraction);
                    }
                    else
                    {
                        if (interception.size() <= subtraction.size())
                        {
                            W.insert(interception);
                        }
                        else
                            W.insert(subtraction);
                    }
                }
            }
        };
        states = {};
        for (auto const &block : P)
        {
            if (block.size() > 0)
            {
                states.insert(*block.begin());
            }
        }
        removeRelated(false);
        for (auto const &block : P)
        {
            if (block.size() > 0)
            {
                std::string first = *block.begin();
                if (block.find(initialState) != block.end() && initialState != first)
                    initialState = first;
                std::vector<DeltaTransition> deltaTransitionTemp = deltaTransitions;
                for (auto &transition : deltaTransitionTemp)
                {
                    if ((block.find(transition.fromState) != block.end() && transition.fromState != first) || (block.find(transition.toState) != block.end() && transition.toState != first))
                    {
                        std::string from;
                        std::string to;
                        if (block.find(transition.fromState) != block.end() && transition.fromState != first)
                        {
                            from = first;
                        }
                        else
                            from = transition.fromState;
                        if (block.find(transition.toState) != block.end() && transition.toState != first)
                        {
                            to = first;
                        }
                        else
                            to = transition.toState;
                        transition.fromState = from;
                        transition.toState = to;
                    }
                }
                deltaTransitions = deltaTransitionTemp;
            }
        }
        std::vector<DeltaTransition> unique = {};
        for (const auto &transition : deltaTransitions)
        {
            if (std::find(unique.begin(), unique.end(), transition) == unique.end())
            {
                unique.push_back(transition);
            }
        }
        deltaTransitions = unique;
        removeRelated();
    };

    void printDka()
    {
        bool printComma;
        printComma = false;
        for (const auto &state : states)
        {
            if (printComma)
                std::cout << ",";
            else
                printComma = true;
            std::cout << state;
        }
        std::cout << std::endl;
        printComma = false;
        for (const auto &input : alphabet)
        {
            if (printComma)
                std::cout << ",";
            else
                printComma = true;
            std::cout << input;
        }
        std::cout << std::endl;
        printComma = false;
        for (const auto &state : acceptableStates)
        {
            if (printComma)
                std::cout << ",";
            else
                printComma = true;
            std::cout << state;
        }
        std::cout << std::endl;
        std::cout << initialState << std::endl;
        for (const auto &transition : deltaTransitions)
        {
            std::cout << transition.fromState << "," << transition.input << "->" << transition.toState << std::endl;
        }
        std::cout << std::endl;
    }
};

std::vector<std::string> tokeniseByDelim(const std::string input, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delim))
        result.push_back(token);

    return result;
}

DeltaTransition createDeltaTransition(const std::string &transition)
{
    std::stringstream ss(transition);
    std::string input, fromState, toState;

    std::getline(ss, fromState, ',');
    std::getline(ss, input, '-');
    std::getline(ss, toState);

    toState = toState.substr(1);

    DeltaTransition deltaTransition = DeltaTransition(input, fromState, toState);
    return deltaTransition;
}

std::set<std::string> getSetFromInput(const std::string &input)
{
    std::vector<std::string> tokenizedVector = tokeniseByDelim(input, ',');
    std::set<std::string> result;
    result.insert(tokenizedVector.begin(), tokenizedVector.end());
    return result;
}

int main()
{
    std::set<std::string> states, alphabet, acceptableStates, transitionFunctions;
    std::string initialState, input, line;

    std::getline(std::cin, input);
    states = getSetFromInput(input);

    std::getline(std::cin, input);
    alphabet = getSetFromInput(input);

    std::getline(std::cin, input);
    acceptableStates = getSetFromInput(input);

    std::getline(std::cin, input);
    initialState = input;

    while (std::getline(std::cin, line))
    {
        if (line == "" || line == "\n")
            break;
        transitionFunctions.insert(line);
    }

    std::vector<DeltaTransition> deltaTransitions;

    for (const auto &transition : transitionFunctions)
    {
        deltaTransitions.push_back(createDeltaTransition(transition));
    }

    DKA dka(states, alphabet, acceptableStates, initialState, deltaTransitions);

    dka.removeUnreachable();
    dka.minimize();
    dka.printDka();

    return 0;
}
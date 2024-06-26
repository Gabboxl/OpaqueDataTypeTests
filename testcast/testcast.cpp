
#include <chrono>
#include <iostream>
#include <format>
#include <string>


struct BASEstr
{
    long long reserved_;
};

typedef struct DATETIMEstr DATETIME;

struct DATETIMEstr
{
    struct BASEstr base_;
};


DATETIME test()
{
    auto now = std::chrono::time_point(std::chrono::system_clock::now());
    DATETIME pDateTime = std::bit_cast<DATETIME>(now);
    //DATETIME pDateTime = *std::bit_cast<DATETIME*>(&now);
    return pDateTime;
}


//usando bit_cast vengono copiati i bit da tipo a tipo in maniera sicura
DATETIME addOneDay(DATETIME* pDateTime)
{
    auto now = std::bit_cast<std::chrono::time_point<std::chrono::system_clock>>(*pDateTime);
    auto tomorrow = now + std::chrono::hours(24);
    DATETIME nextDay = std::bit_cast<DATETIME>(tomorrow);
    return nextDay;
}

// reinterpret_cast utilizzato nella seguente maniera mi permette di accedere direttamente ai bit del DATETIME e modificarli, senza copiarli in un nuovo tipo
void addOneDayModifyBits(DATETIME* pDateTime)
{
    auto now = reinterpret_cast<std::chrono::time_point<std::chrono::system_clock>*>(pDateTime);
    *now += std::chrono::hours(24);
}


void printDate(DATETIME* pDateTime)
{
    auto now = std::bit_cast<std::chrono::time_point<std::chrono::system_clock>>(*pDateTime);

    const std::string s = std::format("{:%FT%T}", now);
    std::cout << s << '\n';
}


int main()
{
    DATETIME x = test();

    //riferimenti a rvalue sono illegali nel c++ ISO, ma consentiti su MSVC (lo erano una volta) disabilitando la conformance mode nel progetto (C/C++ -> Language -> Conformance mode = No)
    DATETIME Y = addOneDay(&test());

    printDate(&Y);


    //-------------------------

    DATETIME z = addOneDay(&x);
    printDate(&x); // non cambia perchè viene restituito una nuova struttura


    addOneDayModifyBits(&x);
    printDate(&x); // cambia perchè sono stati modificati direttamente i bit nella memoria


}

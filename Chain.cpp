#include <iostream>
#include <vector>

using namespace std;
class Handler {
public:
    virtual Handler* SetNext(Handler* handler) = 0;
    virtual std::string Handle(std::string request) = 0;
};
/** Povedenie tsepochki po umolchaniyu moget byt' realizovano vnutri bazovogo klassa obrabotchika.*/
class AbstractHandler : public Handler {
    /**
     * @var Handler
     */
private:
    Handler* next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler* SetNext(Handler* handler) override {
        this->next_handler_ = handler;
        // Vozvrat obrabotchika otsyuda pozvolit svyazat' obrabotchiki prostim sposobom vot tak::
        // $monkey->setNext($squirrel)->setNext($dog);
        return handler;
    }
    std::string Handle(std::string request) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        }

        return {};
    }
};
/** Vse Konkretniye Obrabotchiki libo obrabativayut zapros? libo peredayut ego
* sleduyuschemu obrabotchiku v tsepochke.*/
class MonkeyHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        }
        else {
            return AbstractHandler::Handle(request);
        }
    }
};
class SquirrelHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        }
        else {
            return AbstractHandler::Handle(request);
        }
    }
};
class DogHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + ".\n";
        }
        else {
            return AbstractHandler::Handle(request);
        }
    }
};
/* Obuchno klientskiy kod prisposoblen dlya raboti s edinstvennim obrabotchikom.
* V bol'shinstve sluchayev klienty daghe neizvestno, chto etot obrabotchik yavlyaetsya
* chast'yu tsepochki.*/
void ClientCode(Handler& handler) {
    std::vector<std::string> food = { "Nut", "Banana", "Cup of coffee" };
    for (const std::string& f : food) {
        std::cout << "Client: Who wants a " << f << "?\n";
        const std::string result = handler.Handle(f);
        if (!result.empty()) {
            std::cout << "  " << result;
        }
        else {
            std::cout << "  " << f << " was left untouched.\n";
        }
    }
}
/** Drugaya chast' klientskogo koda sozdayet samy tsepochky.
 */
int main() {
    MonkeyHandler* monkey = new MonkeyHandler;
    SquirrelHandler* squirrel = new SquirrelHandler;
    DogHandler* dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);

    /** Klient dolzhen imet' vozmozhnost' otpravlyat' zapros lyubomy obrabotchiky, a ne
    * tol'ko pervomy v tsepochke.
     */
    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);
    std::cout << "\n";
    std::cout << "Subchain: Squirrel > Dog\n\n";
    ClientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;

    return 0;
}

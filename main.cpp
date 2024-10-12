#include <iostream>
#include "spooler.hpp"
#include "process.hpp"

void startSimulation(unsigned int num_process, unsigned int num_printers) {

    Printer printer1(1);
    Printer printer2(2);
    Printer printer3(3);

    Spooler sp0;  // Sem impressoras
    Spooler sp1(printer1);  // Um objeto Printer lvalue
    Spooler sp2(printer1, printer2, printer3);  // Vários objetos Printer lvalues
    Spooler sp3(10);  // Sem impressoras, mas com capacidade de buffer
    Spooler sp4(10, printer1);  // Um objeto Printer lvalue com buffer_capacity
    Spooler sp5(10, printer1, printer2, printer3);  // Vários objetos Printer lvalues com buffer_capacity

    // Testando com temporários
    sp0.addPrinter(printer1);
//    sp0.addPrinter(Printer(4));
//    Spooler sp6(Printer(4), Printer(5));  // Objetos temporários (rvalues)
//    Spooler sp7(20, Printer(6), Printer(7));

    std::vector<std::pair<Process, std::thread>> process;

    for (int pid = 1; pid <= num_process; pid++) {
        Process p(pid);
        process.emplace_back(p, p.startPrintOrders(sp0));
    }

    for (auto &t: process) {
        if (t.second.joinable()) {
            t.second.join();
        }
    }
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    startSimulation(4, 2);
    return EXIT_SUCCESS;
}

// TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
// <b>lang</b> variable name to see how CLion can help you rename it.

// TIP Press <shortcut actionId="Debug"/> to start debugging your code.
// We have set one <icon src="AllIcons.Debugger.Db_set_breakpoint"/>
// breakpoint for you, but you can always add more by pressing
// <shortcut actionId="ToggleLineBreakpoint"/>.

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
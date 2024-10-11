#include <iostream>
#include "simulator.hpp"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    Simulator::start(4, 2);

    std::cout << timeToString(std::chrono::system_clock::now(), "%H:%M:%S.%s") << std::endl;
    Logger logger;
    logger.print("{} tem {} anos\n", "João", 30);
    logger.print("{} é {}\n", "João", "viado");
    logger.log("{} tem {} anos", "João", 30);
    logger.log(Logger::LogLevel::ERROR, "{} tem {} anos", "João", 30);
    logger.info("{} tem {} anos", "João", 30);
    logger.warn("{} tem {} anos", "João", 30);
    logger.error("{} tem {} anos", "João", 30);
    logger.debug("{} tem {} anos", "João", 30);

    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the
    // <b>lang</b> variable name to see how CLion can help you rename it.

    // TIP Press <shortcut actionId="Debug"/> to start debugging your code.
    // We have set one <icon src="AllIcons.Debugger.Db_set_breakpoint"/>
    // breakpoint for you, but you can always add more by pressing
    // <shortcut actionId="ToggleLineBreakpoint"/>.

    return EXIT_SUCCESS;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
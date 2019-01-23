/* 
 * File:   main.cpp
 * Author: screemer
 *
 * Created on 11 Dec 2018, 20:37
 */

#include <iostream>
#include "database.h"
#include "scanner.h"
#include "splash.h"


using namespace std;

#include "memcard.h"


int scanGames(int argc, char *argv[], Scanner * scanner) {
    Database *db = new Database();
    if (!db->connect(argv[1])) {
        delete db;
        return EXIT_FAILURE;
    }
    if (!db->createInitialDatabase()) {
        cout << "Error creating db structure" << endl;
        db->disconnect();
        delete db;
        return EXIT_FAILURE;
    };


    scanner->scanDirectory(argv[2]);
    scanner->updateDB(db);


    db->disconnect();
    delete db;
    return (EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "USAGE: bleemsync /path/dbfilename.db /path/to/games" << endl;
        return EXIT_FAILURE;
    }



    Scanner *scanner = new Scanner();
    Database *db = new Database();
    if (!db->connect(argv[1])) {
        delete db;
        return EXIT_FAILURE;
    }

    string path = argv[2];

    Memcard *memcardOperation = new Memcard(path);
    memcardOperation->restoreAll(path+Util::separator()+"!SaveStates");


    if (scanner->isFirstRun(path,db))
    {
        scanner->forceScan = true;
    }
    db->disconnect();
    delete db;


    shared_ptr<Splash> splash(Splash::getInstance());
    splash->display(scanner->forceScan);
    splash->drawText("AutoBleem");
    splash->menuSelection();
    splash->saveSelection();
    if (splash->menuOption==MENU_OPTION_SCAN)
    {
        scanGames(argc, argv,scanner);
    }
    splash->logText("Linking shared memcards ...");
    for (DirEntry entry: Util::dir(path)) {
        if (entry.name[0] == '.') continue;
        if (!entry.dir) continue;
        if (entry.name == "!SaveStates") continue;
        if (entry.name == "!MemCards") continue;

        Inifile inifile;
        inifile.load(path+Util::separator()+entry.name+Util::separator()+"Game.ini");
        if (inifile.values["memcard"]!="SONY")
        {
            memcardOperation->backup(path+Util::separator()+"!SaveStates"+Util::separator()+entry.name);
            memcardOperation->swapIn(path+Util::separator()+"!SaveStates"+Util::separator()+entry.name, inifile.values["memcard"]);
        }
    }
    delete memcardOperation;
    splash->logText("Loading ... Please Wait ...");
    splash->finish();
#ifndef NO_GUI
    SDL_Quit();
#endif
    delete scanner;
    return 0;
}



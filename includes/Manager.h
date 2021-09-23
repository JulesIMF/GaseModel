/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Manager.h

Abstract:

    Description of manager, handling molecules behavior.

Author / Creation date:

    JulesIMF / 21.09.21

Revision History:

--*/

#ifndef MOLECULES_MANAGER
#define MOLECULES_MANAGER

//
// Includes / usings
//

#include <vector>
#include <Molecule.h>

//
// Defines
//

struct Manager
{
    Manager(double boundX, double boundY);
    void nextStep(double dt);
    void display(JG::Window &window) const;
    size_t insert(Molecule* molecule);
    Molecule *remove(size_t index);
    void destroy(size_t index);
    size_t size() const;
    double energy() const;
    ~Manager();

protected:
    std::vector<Molecule*> gase;
    void reflectMolecules(Molecule* first, Molecule* second);
    void reflectFromWalls(Molecule* molecule);
    double boundX, boundY;  
    double initEnergy;
};


#endif // !MOLECULES_MANAGER
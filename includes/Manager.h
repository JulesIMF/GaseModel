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
    void display(JG::Canvas& canvas) const;
    size_t insert(Molecule* molecule);
    Molecule *remove(size_t index);
    void destroy(size_t index);
    size_t size() const;
    double energy() const;
    double totalEnergy() const;
    double totalArea() const;
    double totalMass() const;
    void normalize();

    
    void setMinEnergy(MoleculeType first, MoleculeType second, double newMinEnergy);
    void callVirtual(Molecule* first, Molecule* second);
    ~Manager();

    typedef void (Manager::*InteractionFunction)(Molecule *first, Molecule *second);

protected:
    std::vector<Molecule*> gas;
    void reflectFromWalls(Molecule* molecule);
    double boundX, boundY;

    // Normalization
    double initEnergy;
    double initArea;
    double initMass;
    void recalcInit();


    InteractionFunction vTable[nTypesMolecules][nTypesMolecules];
    double minEnergy[nTypesMolecules][nTypesMolecules];
    void updateStates();

    // Interation Functions
    bool underMinEnergy(Molecule* first, Molecule* second);
    void interactBallBall(Molecule* first, Molecule* second);
    void interactSquareSquare(Molecule* first, Molecule* second);
    void interactBallSquare(Molecule* first, Molecule* second);
    void interactSquareBall(Molecule* first, Molecule* second);

    // Debug

    int nTotalBalls = 0;
    
    void countBalls()
    {
        int ans = 0;
        for (auto p : gas)
        {
            if (p->state == MoleculeState::OUT)
                continue;

            if (p->type == (int)MoleculeType::BALL)
                ans++;
            else
                ans += reinterpret_cast<Square*>(p)->nBalls;
        }

        if (!nTotalBalls) nTotalBalls = ans;
        else if(nTotalBalls != ans)
        {
            // printf("Was %d, now %d!\n", nTotalBalls, ans);
            // __asm__("int $3\n");
        }
    }
};


#endif // !MOLECULES_MANAGER
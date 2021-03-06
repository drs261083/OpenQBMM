/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015-2017 Alberto Passalacqua
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::populationBalanceModels::noPopulationBalance

Description
    Disables the solution of the population balance model.

SourceFiles
    noPopulationBalance.C

\*---------------------------------------------------------------------------*/

#ifndef noPopulationBalance_H
#define noPopulationBalance_H

#include "populationBalanceModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace populationBalanceModels
{

/*---------------------------------------------------------------------------*\
                Class noPopulationBalance Declaration
\*---------------------------------------------------------------------------*/

class noPopulationBalance
:
    public populationBalanceModel
{
    // Private data

        //- Name of the noPopulationBalance
        const word name_;


public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        noPopulationBalance
        (
            const word& name,
            const dictionary& dict,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~noPopulationBalance();

    // Member Functions

        //- Return the maximum Courant number ensuring moment realizability
        scalar realizableCo();

        //- Solve population balance equation
        void solve();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace populationBalanceModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

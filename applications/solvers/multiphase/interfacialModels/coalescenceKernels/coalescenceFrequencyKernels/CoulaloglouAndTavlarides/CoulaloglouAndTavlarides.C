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

\*---------------------------------------------------------------------------*/

#include "CoulaloglouAndTavlarides.H"
#include "addToRunTimeSelectionTable.H"
#include "fundamentalConstants.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace coalescenceFrequencyKernels
{
    defineTypeNameAndDebug(CoulaloglouAndTavlarides, 0);

    addToRunTimeSelectionTable
    (
        coalescenceFrequencyKernel,
        CoulaloglouAndTavlarides,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::coalescenceFrequencyKernels::CoulaloglouAndTavlarides::
CoulaloglouAndTavlarides
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    coalescenceFrequencyKernel(dict, mesh),
    fluid_(mesh.lookupObject<twoPhaseSystem>("phaseProperties"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::coalescenceFrequencyKernels::CoulaloglouAndTavlarides::
~CoulaloglouAndTavlarides()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::coalescenceFrequencyKernels::CoulaloglouAndTavlarides::omega
(
    const label nodei,
    const label nodej
) const
{
//     const volScalarField& epsilon = fluid_.phase2().turbulence().epsilon();
    const phaseModel& phase(fluid_.phase1());
    volTensorField S(fvc::grad(phase.U()) + T(fvc::grad(phase.U())));
    volScalarField epsilon(phase.nu()*(S && S));

    const volScalarField& d1 = fluid_.phase1().ds(nodei);
    const volScalarField& d2 = fluid_.phase1().ds(nodej);

    return
        cbrt(epsilon)*sqr(d1 + d2)
       *sqrt(pow(d1, 2.0/3.0) + pow(d2, 2.0/3.0));
}

// ************************************************************************* //

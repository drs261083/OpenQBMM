/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015-2017 Alberto Passalacqua
     \\/     M anipulation  |
-------------------------------------------------------------------------------
2016-06-30 Alberto Passalacqua: Implemented quadrature-based bubbly flow solver.
                                Implementation performed by J. C. Heylmun
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

Application
    polydisperseBubbleFoam

Description
    Solver for polydisperse gas-liquid flows using a quadrature-based moment
    method to describe the gas phase.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "pimpleControl.H"
#include "fvOptions.H"
#include "twoPhaseSystem.H"
#include "PhaseCompressibleTurbulenceModel.H"
#include "fixedValueFvsPatchFields.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"
    #include "createFieldRefs.H"
    #include "createFvOptions.H"
    #include "createTimeControls.H"
    #include "CourantNos.H"
    #include "setInitialDeltaT.H"

    Switch implicitPhasePressure
    (
        mesh.solverDict(alpha1.name()).lookupOrDefault<Switch>
        (
            "implicitPhasePressure", false
        )
    );

    #include "pU/createDDtU.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNos.H"
        #include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        {
            // Transport moments with velocities relative to the mean gas
            // velocity
            fluid.relativeTransport();
            alpha2 = scalar(1) - alpha1;
            alphaPhi2 = phi2*fvc::interpolate(alpha2);
            phi = phase1.alphaPhi() + phase2.alphaPhi();

            // Solve for mean phase velocities and gas volume fraction
            while (pimple.loop())
            {
                fluid.solve();
                fluid.correct();

                #include "contErrs.H"
                #include "pU/DDtU.H"
                #include "pU/UEqns.H"

                #include "pU/pEqn.H"

                if (pimple.turbCorr())
                {
                    // Transport moments with mean gas velocity
                    fluid.averageTransport();
                    alpha2 = scalar(1) - alpha1;
                    alphaPhi2 = phi2*fvc::interpolate(alpha2);
                    phi = phase1.alphaPhi() + phase2.alphaPhi();

                    fluid.correctTurbulence();
                }
            }
        }

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //

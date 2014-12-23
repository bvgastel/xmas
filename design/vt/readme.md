% XMas Verification Tools Design
% Guus Bonnema
% December, 2014

# Verification Tools (VT)

This document reflects the design of verification tools.

## High level VT

The VT tools execute independently from and cooperating with other parts of the
xmas system. In practise this means you can run one or more VT instances from 
the command line, or alternatively as part of the online design tool.

The connection with the online design tool is the coordinator.


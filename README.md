# PagingStructuresRegion
Windows Paging Strutures Region

```

FFFFF700`00000000 |------------| --------------------------------------
									|            |                                      |
									|  PTE High  |                                      |
									|            |                                      |
FFFFF6FB`80000000 |------------| -----------------------------        |
                  |            |                             |        |
                  |  PDE High  |                             |        |
	                |            |                             |        |
FFFFF6FB`7D000000 |------------|------------------           |        |
	                |            |                 |           |        |
	                | PDPTE High |                 |           |        |
FFFFF6FB`7DBEE000 |------------|-----            |           |        |
	                | PML4E      |    | 4KB        | 2MB       | 1GB    | 512GB
FFFFF6FB`7DBED000 |------------|----- PXE_BASE   |           |        |
	                |            |                 |           |        |
	                | PDPTE Low  |                 |           |        |
FFFFF6FB`7DA00000 |------------|------------------ PPE_BASE  |        |
                  |            |                             |        |
                  |  PDE Low   |                             |        |
	                |            |                             |        |
FFFFF6FB`40000000 |------------| ---------------------------- PDE_BASE|
									|            |                                      |
									|  PTE Low   |                                      |
									|            |                                      |
FFFFF680`00000000 |------------| -------------------------------------- PTE_BASE
                      
             Paging Structures Region

```

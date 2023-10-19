# PagingStructuresRegion
Windows Paging Strutures Region
```

FFFF8200`00000000 |------------| --------------------------------------
		              |            |                                      |
		              |  PTE High  |                                      |
                  |            |                                      |
FFFF81C`100000000 |------------| -----------------------------        |
                  |            |                             |        |
                  |  PDE High  |                             |        |
	                |            |                             |        |
FFFF81C0`E0800000 |------------|------------------           |        |
	                |            |                 |           |        |
	                | PDPTE High |                 |           |        |
FFFF81C0`E0704000 |------------|-----            |           |        |
	                | PML4E      |    | 4KB        | 2MB       | 1GB    | 512GB
FFFF81C0`E0703000 |------------|----- PXE_BASE   |           |        |
	                |            |                 |           |        |
	                | PDPTE Low  |                 |           |        |
FFFF81C0`E0600000 |------------|------------------ PPE_BASE  |        |
                  |            |                             |        |
                  |  PDE Low   |                             |        |
	                |            |                             |        |
FFFF81C0`C0000000 |------------| ---------------------------- PDE_BASE|
	                |            |                                      |
		              |  PTE Low   |                                      |
		              |            |                                      |
FFFF8180`00000000 |------------| -------------------------------------- PTE_BASE
                      
             Paging Structures Region

```

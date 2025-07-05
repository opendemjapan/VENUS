int _build_crystal(CELLPARAM cCell,
				 SYMMPOS sPos[MAX_SYMM_POS], int nPos,
				 ATOMTYPE lAtom[MAX_ATOMS_IN_ASYMM], int nAtom,
				 BONDSINI bini[MAX_BONDS_SPEC], int bond_sNum,
				 float range[6],int formatID);

void _get_XYZ_in_range(ATOM BM[MAX_NUM_OF_ATOMS]);


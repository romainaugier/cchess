
def printbb(bb: int) -> None:
    bb &= 1 << 64
    sbb = bin(bb)[3:]
    print("\n".join([sbb[i * 8 : (i + 1) * 8] for i in range(8)]))
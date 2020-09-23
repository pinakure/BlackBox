def test():
    import vpu
    count = 0
    for i in range(1, 1000000):
        a = vpu.createsurf(100,100)
        vpu.deletesurf(a)
        print(f"Surfaces allocated/deallocated: {i}                    ", end="\r")
        #vpu.update()
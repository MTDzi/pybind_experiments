import cmake_example as m


def test_main():
    print('siema')
    assert m.add(1, 2) == 3
    assert m.subtract(1, 2) == -1


if __name__ == '__main__':
    test_main()
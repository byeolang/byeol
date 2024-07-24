#include "../../../namuSyntaxTest.hpp"
#include <chrono>

using namespace nm;
using namespace std;

struct arrTest : public namuSyntaxTest {};

namespace {
    class myNode : public node {
        NM(CLASS(myNode, node))

    public:
        myNode(): number(0) {}
        myNode(int num): number(num) {}

        scope& subs() override { return nulOf<scope>(); }
        priorType prioritize(const args& a) const override { return NO_MATCH; }
        using super::run;
        str run(const args& a) override { return str(); }

        int number;
    };

    class myMyNode : public myNode {
        NM(CLASS(myMyNode, myNode))

    public:
        myMyNode(int num): super(num) {}
    };

    void benchMarkArr(int cnt) {
        vector<str> vec;

        auto start = chrono::steady_clock::now();
        for(int n=0; n < cnt; n++) {

            vec.push_back(str(new myNode(n)));
        }
        int sz = vec.size();
        auto startDeleting = chrono::steady_clock::now();
        vec.clear();
        auto end = chrono::steady_clock::now();

        auto addingElapsed = startDeleting - start;
        auto removingElapsed = end - startDeleting;
        auto totalElapsed = end - start;

        NM_I("[benchMarkArr]: vector took total %d ms for adding(%dms) & removing(%dms) of %d elems.",
             (nint64) (totalElapsed / chrono::milliseconds(1)), (nint64) (addingElapsed / chrono::milliseconds(1)),
             (nint64) (removingElapsed / chrono::milliseconds(1)), sz);

        arr arr1;
        start = chrono::steady_clock::now();
        for(int n=0; n < cnt; n++) {
            arr1.add(*(new myNode(n)));
        }
        sz = arr1.len();
        startDeleting = chrono::steady_clock::now();
        arr1.rel();
        end = chrono::steady_clock::now();

        addingElapsed = startDeleting - start;
        removingElapsed = end - startDeleting;
        totalElapsed = end - start;

        NM_I("[benchMarkArr]: arr took total %d ms for adding(%dms) & removing(%dms) of %d elems.",
             (nint64) (totalElapsed / chrono::milliseconds(1)), (nint64) (addingElapsed / chrono::milliseconds(1)),
             (nint64) (removingElapsed / chrono::milliseconds(1)), sz);
    }
}

TEST_F(arrTest, instantiateTest) {
    arr arr1;
}

TEST_F(arrTest, shouldNotCanAddLocalObject) {
    tarr<myNode> arr1;
    ASSERT_EQ(arr1.len(), 0);

    {
        myNode localObj(5);
        ASSERT_TRUE(arr1.add(localObj));
        ASSERT_FALSE(nul(arr1[0]));
        ASSERT_EQ(arr1.len(), 1);
    }

    ASSERT_EQ(arr1.len(), 1);
    auto& elem = arr1[0];
    ASSERT_TRUE(nul(elem));
}

TEST_F(arrTest, simpleAddDelTest) {
    tarr<myNode> arr1;
    ASSERT_EQ(arr1.len(), 0);

    const int EXPECT_NUMBER = 5;
    arr1.add(*(new myNode(EXPECT_NUMBER)));
    ASSERT_EQ(arr1.len(), 1);

    auto elem1 = arr1[0].cast<myNode>();
    ASSERT_FALSE(nul(elem1));
    ASSERT_EQ(elem1.number, EXPECT_NUMBER);
}

TEST_F(arrTest, addDel10Elems) {
    tarr<myNode> arr1;
    const int cnt = 10;
    for(int n=0; n < cnt; n++) {
        ASSERT_TRUE(arr1.add(*(new myNode(n))));
    }

    ASSERT_EQ(arr1.len(), cnt);
}

TEST_F(arrTest, benchMarkArrTest) {
    benchMarkArr(100);
    benchMarkArr(1000);
    benchMarkArr(10000);
}

TEST_F(arrTest, testIter) {
    arr arr1;
    arr1.add(new myNode(0));
    arr1.add(new myNode(1));
    arr1.add(new myNode(2));

    auto e = arr1.begin();
    auto head = e++;
    auto index2 = ++e;

    EXPECT_TRUE(arr1.begin()+2 == index2);
    EXPECT_TRUE(arr1.begin() == head);

    ASSERT_EQ(e.next(1), 0);
}

TEST_F(arrTest, testContainableAPI) {
    //  initial state:
    tarr<myNode>* arr1 = new tarr<myNode>();
    ASSERT_EQ(arr1->len(), 0);

    auto head = arr1->begin();
    ASSERT_TRUE(head.isEnd());
    auto tail = arr1->end();
    ASSERT_TRUE(tail.isEnd());

    ASSERT_TRUE(arr1->add(arr1->begin(), new myNode(0)));
    ASSERT_TRUE(arr1->add(arr1->end(), new myMyNode(1)));
    ASSERT_EQ(arr1->len(), 2);

    // add:
    int expectVal = 0;
    for(auto e=arr1->begin(); e != arr1->end() ;e++) {
        myNode& elem = e->cast<myNode>();
        ASSERT_FALSE(nul(elem));
        ASSERT_EQ(elem.number, expectVal++);
    }

    // get & each:
    expectVal = 0;
    for(int n=0; n < arr1->len() ;n++) {
        myNode& elem = arr1->get(n).cast<myNode>();
        ASSERT_FALSE(nul(elem));
        ASSERT_EQ(elem.number, expectVal++);
    }

    {
        tnarr<myNode> tray = arr1->getAll<myNode>([](const myNode& elem) {
            return true;
        });
        ASSERT_EQ(tray.len(), 2);

        int cnt = 0;
        tray = arr1->getAll<myNode>([&cnt](const myNode& elem) {
            if(cnt++ >= 1) return false;
            return true;
        });
        ASSERT_EQ(tray.len(), 1);
    }

    myMyNode& tray = arr1->get<myMyNode>([](const myMyNode& elem) {
        if(elem.number == 1) return true;
        return false;
    });
    ASSERT_FALSE(nul(tray));

    //  del:
    ASSERT_TRUE(arr1->del());
    ASSERT_EQ(arr1->len(), 1);
    ASSERT_EQ(arr1->get(0).cast<myNode>().number, 0);
    delete arr1;
}

TEST_F(arrTest, testcloneDeep) {
    narr arr1;
    arr1.add(new myNode(1));
    arr1.add(new myNode(2));
    ASSERT_EQ(arr1.len(), 2);

    tstr<narr> arr2wrap((narr*) arr1.cloneDeep());
    ASSERT_TRUE(arr2wrap);
    narr& arr2 = *arr2wrap;
    ASSERT_EQ(arr1.len(), arr2.len());

    for(int n=0; n < 2; n++) {
        ASSERT_EQ(arr1[n].cast<myNode>().number, arr2[n].cast<myNode>().number);
        ASSERT_NE(&arr1[n], &arr2[n]);
    }
}

TEST_F(arrTest, testRangeBasedForLoop) {

    narr arr1;
    arr1.add(new myNode(3));
    arr1.add(new myNode(7));

    int sum = 0;
    for(auto& e : arr1) {
        myNode& cast = e.cast<myNode>();
        sum += cast.number;
    }

    int sum2 = 0;
    for(const node& e : arr1) {
        const myNode& cast = e.cast<myNode>();
        sum2 += cast.number;
    }
    ASSERT_EQ(sum2, sum);

    int expect = 0;
    for(int n=0; n < arr1.len(); n++)
        expect += arr1[n].cast<myNode>().number;

    ASSERT_EQ(sum, expect);
}

TEST_F(arrTest, testSubs) {
    tarr<nInt> intArr;
    ASSERT_NE(intArr.subs().len(), 0);
    intArr.add(new myNode(1));

    str res = intArr.run("len");
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), intArr.len());

    nInt int1;
    arr intArr2(int1);
    // even if instances of arr are different, they share same subs because of same elementType:
    ASSERT_EQ(&intArr.sub("len"), &intArr2.sub("len"));
}

TEST_F(arrTest, genericMarshalingTest) {
    nInt int1;
    arr arr1(int1);

    baseFunc& f = arr1.sub<baseFunc>("add", args{narr{int1}});
    ASSERT_FALSE(nul(f));

    nFlt flt1;
    tarr<nFlt> arr2;
    baseFunc& f2 = arr2.sub<baseFunc>("add", args{narr{flt1}});
    ASSERT_FALSE(nul(f2));
    const param& fltParam = f2.getParams()[0];
    ASSERT_FALSE(nul(fltParam));
    ASSERT_TRUE(fltParam.getOrigin().isSub<nFlt>());
}

TEST_F(arrTest, testSimpleBridgedFuncs) {
    tarr<nInt> arr1;
    node& it = arr1;

    str res = it.run("add", args{narr{*new nInt(1)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), true);
    ASSERT_EQ(arr1.len(), 1);
    res = it.run("len");
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, testSimpleBridgedFuncs2) {
    tarr<nInt> arr1;
    node& it = arr1;

    str res = it.run("add", args{narr{*new nInt(1)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), true);
    res = it.run("add", args{narr{*new nInt(0), *new nInt(2)}}); // arr: {2, 1}

    ASSERT_EQ(arr1.len(), 2);
    res = it.run("len");
    ASSERT_EQ(res.cast<nint>(), 2);

    res = it.run("get", args{narr{*new nInt(0)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
    ASSERT_EQ(arr1[0].cast<nint>(), 2);

    res = it.run("set", args{narr{*new nInt(1), *new nInt(2)}}); // arr: {2, 2}
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), true);
    ASSERT_EQ(arr1[0].cast<nint>(), arr1[1].cast<nint>());

    res = it.run("has", args{narr{*new nInt(0)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), false);
    res = it.run("has", args{narr{*new nInt(1)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), false);
    res = it.run("has", args{narr{*new nInt(2)}});
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nbool>(), true);
}

TEST_F(arrTest, testIteratorBridgedFunc) {
    tarr<nInt> arr1;
    node& it = arr1;

    arr1.add(new nInt(1));

    str res = it.run("iterate", args{narr{*new nInt(0)}});
    ASSERT_TRUE(res);
    ASSERT_NE(res->subs().len(), 0);

    str resOfIter = res->run("isEnd");
    ASSERT_TRUE(resOfIter);
    ASSERT_FALSE(resOfIter->cast<nbool>());

    str elem = res->run("get");
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->cast<nint>(), 1);
    ASSERT_EQ(arr1.getType().getBeans().len(), 1);
    ASSERT_EQ(elem->getType(), arr1.getType().getBeans()[0].getType());

    str step = res->run("next", args{narr{*new nInt(1)}});
    ASSERT_TRUE(step);
    ASSERT_EQ(step->cast<nint>(), 0);

    resOfIter = res->run("isEnd");
    ASSERT_TRUE(resOfIter);
    ASSERT_TRUE(resOfIter->cast<nbool>());
}

TEST_F(arrTest, newInstanceSharesFuncs) {
    arr a(*new nInt(0));
    ASSERT_TRUE(a.canRun(args{}));
    tstr<arr> res = a.run();
    ASSERT_FALSE(nul(res));

    ASSERT_TRUE(res->canRun(args{}));
    ASSERT_EQ(a.subs().len(), res->subs().len());
    ASSERT_EQ(&a.subs(), &res->subs()); // if type is same, subs should be same too.

    arr b(*new nFlt(0.0f));
    tstr<arr> res2 = b.run();
    ASSERT_TRUE(res2);

    ASSERT_EQ(res2->subs().len(), res->subs().len());
    ASSERT_NE(&res2->subs(), &res->subs());

    ASSERT_EQ(res2->len(), 0);
    res2->run("add", narr{*new nStr("1")});
    ASSERT_EQ(res2->len(), 0);
    res2->add(new nFlt(1.0f));
    ASSERT_EQ(res2->len(), 1);
    ASSERT_EQ(res2->get(0).cast<nflt>(), 1.0f);
}

TEST_F(arrTest, testBasicDefSyntax) {
    make().parse(R"SRC(
        print(msg str) void: 1

        getIndex() int
            ret 1

        main() int
            arr int[]
            print("len=" + arr.len() as str)
            arr.add(1)
            arr.add(2.5)
            print("len=" + arr.len() as str + "\n")
            print("arr[0]=" + arr.get(0) as str + "\n")
            print("arr[1]=" + arr[1] as str + "\n")
            ret arr[
                getIndex()]
    )SRC").shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 2); // 2.5(flt) --> 2(int)
}

TEST_F(arrTest, testImplicitlyDefSyntax) {
    make().parse(R"SRC(
        print(msg str) void: 1

        sum(arr int[]) int
            ret arr[0] + arr[1] + arr[2]

        main() int
            s := sum({1, 2, 3})
            print("sum = " + s as str + "\n")
            ret s
    )SRC").shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 6);
}

TEST_F(arrTest, testImplicitlyefSyntax2) {
    make().parse(R"SRC(
        sum(arr flt[]) flt
            ret arr[0] + arr[2]

        main() int
            g := {1.5, 3, 5.5}
            s := sum(g)
            ret s as int
    )SRC").shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 7);
}

TEST_F(arrTest, arrDeductionFailNegative) {
    make().negative().parse(R"SRC(
        def obj
            name := "kniz"

        main() void
            arr := {1, 2.5, obj()}
            print(arr.len())
    )SRC").shouldVerified(false);

    errReport& errs = getReport();
    ASSERT_TRUE(errs);
    ASSERT_EQ(errs[0].pos.row, 6);
}

TEST_F(arrTest, arrDeductionFailNegative2) {
    make().negative().parse(R"SRC(
        def obj
            name := "kniz"

        main() void
            arr := {1, 2.5, obj()}
    )SRC").shouldVerified(false);
}

TEST_F(arrTest, arrDeductionFailNegative3) {
    make().negative().parse(R"SRC(
        def obj
            name := "kniz"

        main() void
            arr := {1, 2.5, obj()}
            print(arr[0].name)
    )SRC").shouldVerified(false);
}

TEST_F(arrTest, arrDeductionFailNegative4) {
    make().negative().parse(R"SRC(
        def obj
            name := "kniz"
        def obj2
            name := "kniz"
        main() void
            arr := {obj(), obj2()}
    )SRC").shouldVerified(false);
}

TEST_F(arrTest, addFromEmptyArray) {
    make().parse(R"SRC(
        def person
            value str
        main() int
            arr person[]
            for n in 1..20
                elem := person()
                elem.value = n as str
                arr.add(elem)

            print("start...")
            res := ""
            for elem in arr
                res = res + elem.value
            res.len()
    )SRC").shouldVerified(true);
}

TEST_F(arrTest, testDeepCopy) {
    make().parse(R"SRC(
        main() int
            org := {1, 2}
            copy := org(org)
            org.set(1, 3)
            ret copy[1]
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 2);
}

TEST_F(arrTest, testShallowCopy) {
    make().parse(R"SRC(
        main() int
            org := {1, 2}
            copy := org
            org.set(1, 3)
            ret copy[1]
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res->cast<nint>(), 3);
}

TEST_F(arrTest, test2DArray) {
    make().parse(R"SRC(
        main() int
            arr := {{0, 1}, {1, 2}}
            ret arr[0][1]
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, test2DArray2) {
    make().parse(R"SRC(
        main() int
            arr := {{0, 1}, {1, 2}}
            arr3 int[][]
            arr3 = arr
            arr[1].set(1, 4)
            arr3[1][1]
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 4);
}

TEST_F(arrTest, test2DArray3) {
    make().parse(R"SRC(
        print(msg str) void: 1

        foo(m byte[]) void
            for n in 0..m.len()
                print(m[n] as str)
            print("\n")

        main() int
            msgs := {{0 as byte, 1 as byte, 2 as byte, 3 as byte, 4 as byte}, {5 as byte, 6 as byte, 7 as byte, 8 as byte, 9 as byte}}
            for n in 0..msgs.len()
                foo(msgs[n])
            ret 0
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(arrTest, testCallCtor) {
    make().parse(R"SRC(
        main() int
            a := {1, 2, 3}
            a().len()
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 0);
}

TEST_F(arrTest, addDifferentElemTypeNegative) {
    make().negative().parse(R"SRC(
        main() void
            map := {{"hello", "world"}, {"I'm", "kniz"}}
            f int[]
            f.add(1)
            f.add(2)
            map.add(f)
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(arrTest, namelessArr) {
    make().parse(R"SRC(
        main() int
            {"hello", "world"}.len()
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 2);
}

TEST_F(arrTest, accessElementOnce) {
    make().parse(R"SRC(
        def p
            cnt := 0
            arr := {"h", "e"}
            foo() str[]
                cnt++
                arr
        main() int
            p1 p
            p1.foo()[1]
            p1.cnt
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1); // shouldn't be 2.
}

TEST_F(arrTest, setElemConversion) {
    make().parse(R"SRC(
        main() int
            arr := {'a', 'c'}
            arr[1] = 'd'
            arr[1] == 'd'
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, setElemConversionNegative) {
    make().negative().parse(R"SRC(
        foo() int
            3
        main() int
            arr := {'a', 'c'}
            arr[1] = foo()
            arr[1] == 'd'
    )SRC").shouldVerified(false);
}

TEST_F(arrTest, setElemConversion1) {
    make().parse(R"SRC(
        foo() int
            0
        boo() char
            'c'
        main() int
            arr := {'a', 'c'}
            arr[foo()] = (boo() + 1) as char
            arr[0] == 'd'
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, setElemAddAssignConversion) {
    make().parse(R"SRC(
        foo() int
            0
        boo() int
            1
        main() int
            arr := {1, 2}
            arr[foo()] += boo()
            arr[0] == 2
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, setElemAddAssignConversion2) {
    make().parse(R"SRC(
        foo() int
            1
        boo() int
            3
        main() int
            arr := {6, 9}
            arr[foo() - 1] /= boo()
            arr[0] == 2
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, setFieldOfElemAfterGetIt) {
    make().parse(R"SRC(
        def A
            age int
        main() int
            a A[]
            a.add(A())
            a[0].age = 22
            ret a[0].age == 22
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 1);
}

TEST_F(arrTest, setFieldOfElemAfterGetIt1) {
    make().parse(R"SRC(
        def A
            age int
        main() int
            a A[]
            a.add(A())
            a[0].age = 22
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(res.cast<nint>(), 22);
}

TEST_F(arrTest, outOfBoundExOccurs) {
    make().parse(R"SRC(
        def A
            arr str[]
            foo() str
                arr.add("hello")
                arr[2] // ex occurs here!
        main() void
            a A
            print(a.foo())
    )SRC").shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(nul(res.cast<nint>())); // which means, program ended with error code.
    err& resErr = res.cast<err>();
    ASSERT_FALSE(nul(resErr));
    ASSERT_EQ(resErr.code, errCode::OUT_OF_RANGE);

    {
        auto& A = getSubPack().sub("A"); // A.arr is mockNode
        str a((node*) A.clone()); // now, a.arr is not mockNode, but obj.
        threadUse th;
        str res = a->run("foo");
        ASSERT_TRUE(res);
        err& cast = res->cast<err>();
        ASSERT_FALSE(nul(cast));
        ASSERT_EQ(cast.fType, logLv::ERR);
        ASSERT_EQ(cast.code, errCode::OUT_OF_RANGE);
    }
}

#include <chrono>

#include "test/namuTest.hpp"

using namespace nm;
using namespace std;

struct nmapTest: public namuTest {};

namespace {
    class myNode: public node {
        NM(CLASS(myNode, node))

    public:
        myNode(int num): number(num) {}

        scope& subs() override { return dumScope::singleton(); }

        priorType prioritize(const args& types) const override { return NO_MATCH; }

        str run(const args& a) override { return str(); }

        int number;
    };

    void benchMarkNMap(int cnt) {
        map<std::string, str> vec;

        auto start = chrono::steady_clock::now();
        for(int n = 0; n < cnt; n++)
            vec.insert({"hello", str(new myNode(n))});
        int sz = vec.size();
        auto startDeleting = chrono::steady_clock::now();
        vec.clear();
        auto end = chrono::steady_clock::now();

        auto addingElapsed = startDeleting - start;
        auto removingElapsed = end - startDeleting;
        auto totalElapsed = end - start;

        NM_I("[benchMarkNMap]: map took total %d ms for adding(%dms) & removing(%dms) of %d elems.",
            (nint64) (totalElapsed / chrono::milliseconds(1)),
            (nint64) (addingElapsed / chrono::milliseconds(1)),
            (nint64) (removingElapsed / chrono::milliseconds(1)), sz);


        nmap map1;
        start = chrono::steady_clock::now();
        for(int n = 0; n < cnt; n++)
            map1.add("hello", *(new myNode(n)));
        sz = map1.len();
        startDeleting = chrono::steady_clock::now();
        map1.rel();
        end = chrono::steady_clock::now();

        addingElapsed = startDeleting - start;
        removingElapsed = end - startDeleting;
        totalElapsed = end - start;

        NM_I(
            "[benchMarkNMap]: nmap took total %d ms for adding(%dms) & removing(%dms) of %d elems.",
            (nint64) (totalElapsed / chrono::milliseconds(1)),
            (nint64) (addingElapsed / chrono::milliseconds(1)),
            (nint64) (removingElapsed / chrono::milliseconds(1)), sz);
    }

    class myMyNode: public myNode {
        NM(CLASS(myMyNode, myNode))

    public:
        myMyNode(int num): super(num) {}
    };
} // namespace

TEST_F(nmapTest, instantiateTest) { nmap map1; }

TEST_F(nmapTest, shouldNotCanAddLocalObject) {
    tnmap<std::string, myNode> map1;
    ASSERT_EQ(map1.len(), 0);

    {
        myNode localObj(5);
        ASSERT_TRUE(map1.add("local", localObj));
        ASSERT_TRUE(map1["local"]);
        ASSERT_EQ(map1.len(), 1);
    }

    ASSERT_EQ(map1.len(), 1);
    ASSERT_TRUE(map1.in("local"));
}

TEST_F(nmapTest, simpleAddDelTest) {
    tnmap<std::string, myNode> map1;
    ASSERT_EQ(map1.len(), 0);

    const int EXPECT_NUMBER = 5;
    map1.add("5", *(new myNode(EXPECT_NUMBER)));
    ASSERT_EQ(map1.len(), 1);

    auto& elem1 = map1["5"];
    ASSERT_FALSE(nul(elem1));
    ASSERT_EQ(elem1.number, EXPECT_NUMBER);
}

TEST_F(nmapTest, addDel10Elems) {
    tnmap<float, myNode> map1;
    const float cnt = 10;
    for(int n = 0; n < cnt; n++)
        ASSERT_TRUE(map1.add((float) n, *(new myNode(n))));

    ASSERT_EQ(map1.len(), cnt);
}

TEST_F(nmapTest, benchMarkNMapTest) {
    benchMarkNMap(100);
    benchMarkNMap(1000);
    benchMarkNMap(10000);
}

TEST_F(nmapTest, testIter) {
    nmap map1;
    map1.add("0", new myNode(0));
    map1.add("1", new myNode(1));
    map1.add("2", new myNode(2));

    auto e = map1.begin();
    auto head = e++;
    auto index2 = ++e;

    EXPECT_TRUE(map1.begin() + 2 == index2);
    EXPECT_TRUE(map1.begin() == head);

    ASSERT_FALSE(e.isEnd());
    ASSERT_EQ(std::to_string(e.getVal<myNode>()->number), e.getKey());
    ASSERT_EQ(e.next(1), 0);
}

nbool hasSequentialValueOf(int val, tbicontainable<std::string, myNode>& from) {
    std::vector<int> tray;
    for(auto& elem: from)
        tray.push_back(elem.number);

    for(int n = 0; n < val; n++) {
        nbool found = false;
        for(int n2 = 0; n < tray.size(); n2++)
            if(tray[n2] == n) {
                found = true;
                break;
            }
        if(!found) return false;
    }
    return true;
}

TEST_F(nmapTest, testucontainableAPI) {
    //  initial state:
    tnmap<std::string, myNode>* map1 = new tnmap<std::string, myNode>();
    tbicontainable<std::string, myNode>* con = map1;
    ASSERT_EQ(con->len(), 0);

    auto head = con->begin();
    ASSERT_TRUE(head.isEnd());
    auto tail = con->end();
    ASSERT_TRUE(tail.isEnd());

    ASSERT_TRUE(con->add("0", new myNode(0)));
    ASSERT_TRUE(con->add("end", new myMyNode(1)));
    ASSERT_EQ(con->len(), 2);

    // add:
    ASSERT_TRUE(hasSequentialValueOf(1, *con));
    ASSERT_TRUE(hasSequentialValueOf(1, *con));

    {
        tnarr<myNode> tray =
            map1->getAll<myNode>([](const std::string&, const myNode& elem) { return true; });
        ASSERT_EQ(tray.len(), 2);

        int cnt = 0;
        tray = map1->getAll<myNode>([&cnt](const std::string&, const myNode& elem) {
            if(cnt++ >= 1) return false;
            return true;
        });
        ASSERT_EQ(tray.len(), 1);
    }

    ASSERT_TRUE(map1->get<myMyNode>([](const std::string&, const myMyNode& elem) {
        if(elem.number == 1) return true;
        return false;
    }));

    //  del:
    ASSERT_TRUE(con->del("end"));
    ASSERT_EQ(con->len(), 1);
    ASSERT_EQ(map1->get("0")->number, 0);

    //  add with element:
    tnmap<std::string, myNode> map2;
    ASSERT_EQ(map2.add(*con), 1);
    ASSERT_TRUE(map2.add("1", new myNode(1)));
    ASSERT_TRUE(map2.add("2", new myMyNode(2)));
    ASSERT_TRUE(map2.add("3", new myNode(3)));
    ASSERT_EQ(map2["2"].number, 2);
    ASSERT_EQ(map2["3"].number, 3);
    ASSERT_EQ(map2.len(), 4);

    auto e = map2.begin();
    e = e + 2;
    ASSERT_EQ(e.getKey(), std::to_string(e.getVal<myNode>()->number));
    ASSERT_TRUE(map2.add("5", new myNode(5)));
    ASSERT_TRUE(map2.add("6", new myNode(6)));

    ASSERT_EQ(map2["0"].cast<myNode>()->number, 0);
    ASSERT_EQ(map2["1"].cast<myNode>()->number, 1);
    ASSERT_EQ(map2["6"].cast<myNode>()->number, 6);
    ASSERT_EQ(map2["5"].cast<myNode>()->number, 5);
    ASSERT_EQ(map2["2"].cast<myNode>()->number, 2);
    ASSERT_EQ(map2["3"].cast<myNode>()->number, 3);

    ASSERT_EQ(con->len(), 1);
    ASSERT_EQ(con->add(map2.iterate(1), map2.iterate(3)), 2);
    ASSERT_EQ(con->len(), 3);
    con->rel();
    ASSERT_TRUE(con->len() == 0);

    ASSERT_EQ(con->add(map2.begin() + 2, map2.end()), 4);
    ASSERT_EQ(con->len(), 4);
    ASSERT_TRUE(con->del(con->begin() + 1, con->begin() + 3));
    ASSERT_EQ(con->len(), 2);

    delete con;
}

TEST_F(nmapTest, testRangeBasedForLoop) {
    nmap map1;
    map1.add("3", new myNode(3));
    map1.add("7", new myNode(7));

    int sum = 0;
    for(auto& e: map1) {
        myNode& cast = *e.cast<myNode>();
        sum += cast.number;
    }

    int sum2 = 0;
    for(const node& e: map1) {
        const myNode& cast = *e.cast<myNode>();
        sum2 += cast.number;
    }
    ASSERT_EQ(sum2, sum);
}

TEST_F(nmapTest, addMultipleKey) {
    nmap m;
    m.add("1", new myNode(1));
    m.add("2", new myNode(2));
    m.add("1", new myNode(3));
    m.add("3", new myNode(4));

    ncnt key1found = 0;
    for(auto e = m.begin(); e; ++e)
        if(*e.getKey() == "1") key1found++;
    ASSERT_EQ(key1found, 2);
}

TEST_F(nmapTest, searchMultipleKey) {
    nmap m;
    m.add("1", new myNode(1));
    m.add("2", new myNode(2));
    m.add("1", new myNode(3));
    m.add("3", new myNode(4));

    nint val = m.get("1")->cast<myNode>()->number;
    ASSERT_TRUE(val == 1 || val == 3);

    auto founds = m.getAll("1");
    ASSERT_EQ(founds.len(), 2);
    auto e = founds.begin();
    nint val1 = e++.get<myNode>()->number, val2 = e.get<myNode>()->number;
    ASSERT_TRUE(val1 == 1 || val1 == 3);
    ASSERT_TRUE(val2 == 1 || val2 == 3);
    ASSERT_NE(val1, val2);
}

TEST_F(nmapTest, testDeletionByKey) {
    nmap m;
    m.add("1", new myNode(1));
    m.add("2", new myNode(2));
    m.add("1", new myNode(3));

    ASSERT_TRUE(m.del("1"));
    ASSERT_EQ(m.len(), 1);

    auto e = m.begin();
    ASSERT_EQ(e.getKey(), std::string("2"));
    ASSERT_EQ(e.getVal<myNode>()->number, 2);
}

TEST_F(nmapTest, testDeletionByIter) {
    nmap m;
    m.add("1", new myNode(1));
    m.add("2", new myNode(2));
    m.add("1", new myNode(3));

    auto e = m.begin();
    while(*e.getKey() == "1")
        ++e;
    ASSERT_TRUE(m.del(e));
    ASSERT_EQ(m.len(), 2);

    e = m.begin();
    ASSERT_EQ(e.getKey(), "1");
    nint val1 = e++.getVal<myNode>()->number, val2 = e.getVal<myNode>()->number;
    ASSERT_EQ(e.getKey(), "1");

    ASSERT_TRUE(val1 == 1 || val1 == 3);
    ASSERT_TRUE(val2 == 1 || val2 == 3);
    ASSERT_NE(val1, val2);
}

TEST_F(nmapTest, testSetValue) {
    nmap m;
    m.add("1", new myNode(1));
    m.add("2", new myNode(2));
    m.add("1", new myNode(3));

    m.iterate("2").setVal(new myNode(4));
    nbool found = false;
    for(auto& e: m)
        if(e.cast<myNode>()->number == 4) found = true;
    ASSERT_TRUE(found);

    for(auto e = m.iterate("1"); e; ++e)
        if(e.getVal<myNode>()->number == 3) e.setVal(new myNode(5));

    found = false;
    for(auto e = m.iterate("1"); e; ++e)
        if(e.getVal<myNode>()->number == 5) found = true;
    ASSERT_TRUE(found);
}

TEST_F(nmapTest, delWhileIteration) {
    nmap m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    m.add("apple", new nInt(3));
    m.add("banana", new nInt(4));
    m.add("meat", new nInt(5));
    m.add("banana", new nInt(6));
    m.add("meat", new nInt(7));
    m.add("banana", new nInt(8));

    for(auto e = m.begin(); e;)
        if(*e.getKey() == "banana") m.del(e++);
        else ++e;

    ASSERT_EQ(m.len(), 4);
    ASSERT_EQ(m.get("apple")->cast<int>(), 3);
    ASSERT_EQ(m.getAll("meat").len(), 3);
}

TEST_F(nmapTest, testEach) {
    nmap map1;
    map1.add("val1", *new nInt(1));
    map1.add("val2", *new nByte(100));
    map1.add("val3", *new nInt(2));

    nint sum = 0;
    map1.each<nInt>([&](const std::string&, const nInt& elem) { return sum += elem.get(), true; });
    ASSERT_EQ(sum, 3);

    auto arr2 = map1.getAll<nInt>(
        [&](const std::string&, const auto& elem) -> nbool { return elem.get() > 1; });
    ASSERT_EQ(arr2.len(), 1);
    ASSERT_EQ(arr2[0].get(), 2);
}

TEST_F(nmapTest, insertionOrderShouldBeKept) {
    nmap map1; // nmap is using smultimap, sequential multi map.
               // it's a sort of a map but rememebers insertion order.
    map1.add("1", *new nInt(1));
    map1.add("2", *new nInt(2));
    map1.add("3", *new nInt(3));
    map1.add("4", *new nInt(4));

    int expects[] = {1, 2, 3, 4};
    int n = 0;
    for(const auto& val: map1)
        ASSERT_EQ(val.cast<nInt>()->get(), expects[n++]);

    n = 0;
    map1.each<nInt>([&](const auto& key, const nInt& val) -> nbool {
        EXPECT_EQ(val.get(), expects[n++]);
        return true;
    });
}

TEST_F(nmapTest, simpleReversedIterator) {
    nmap map1;
    map1.add("1", *new nInt(1));
    map1.add("2", *new nInt(2));
    map1.add("3", *new nInt(3));
    map1.add("4", *new nInt(4));

    int expects[] = {4, 3, 2, 1};
    auto re = map1.rbegin();
    ASSERT_FALSE(re.isEnd());
    for(int n = 0; n < 4; n++, ++re)
        ASSERT_EQ(re->cast<nint>(), expects[n]);
}

TEST_F(nmapTest, stepForwardReversedIterator) {
    nmap map1;
    map1.add("1", *new nInt(1));
    map1.add("2", *new nInt(2));
    map1.add("3", *new nInt(3));
    map1.add("4", *new nInt(4));

    auto re = map1.rbegin();
    ASSERT_TRUE(re.isReversed());
    ASSERT_EQ(re->cast<nint>(), 4);
    ASSERT_EQ(re.getKey(), "4");

    ++re; // 3
    ++re; // 2
    ASSERT_EQ(re->cast<nint>(), 2);

    re.stepForward(2); // 4
    ASSERT_EQ(re->cast<nint>(), 4);

    re.stepBackward(1); // 3
    ASSERT_EQ(*re.getVal<nInt>()->cast<nint>(), 3);

    ASSERT_EQ(re.next(6), 2);
    ASSERT_TRUE(re.isEnd());
    ASSERT_EQ(re, map1.rend());
}

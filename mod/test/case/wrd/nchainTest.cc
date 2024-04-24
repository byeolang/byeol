#include "../../common/dep.hpp"

using namespace wrd;
using namespace std;

namespace {
    class myNode : public node {
        WRD(CLASS(myNode, node))

    public:
        myNode(int num): number(num) {}

        nbicontainer& subs() override { return nulOf<nbicontainer>(); }
        wbool canRun(const ucontainable& types) const override { return false; }
        str run(const ucontainable& args) override { return str(); }

        int number;
    };

    void simpleAddDelTest(int cnt) {
        tnmap<std::string, myNode>* m = new tnmap<std::string, myNode>();
        tnchain<std::string, myNode> chn(*m);
        ASSERT_EQ(0, m->len());
        ASSERT_EQ(chn.len(), m->len());
        ASSERT_TRUE(nul(chn.getNext()));

        std::map<std::string, myNode*> tray;
        for(int n=0; n < cnt ;n++) {
            myNode* new1 = new myNode(n);
            m->add("name" + std::to_string(n), new1);
            tray.insert({"name" + std::to_string(n), new1});
        }

        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);

        ASSERT_TRUE(nul(chn.getNext()));
        for(auto e=chn.begin(); e ;++e) {
            ASSERT_FALSE(nul(e.getKey()));
            ASSERT_NE(e.getKey(), "");
            const myNode& elem = e.getVal();
            const myNode& answer = *tray[e.getKey()];
            ASSERT_EQ(&elem, &answer);
            ASSERT_EQ("name" + std::to_string(elem.number), e.getKey());
        }
    }

    class myMyNode : public myNode {
        WRD(CLASS(myMyNode, myNode))

    public:
        myMyNode(int num): super(num) {}
    };

    void examineChain2Element(nchain& chn, int val1, int val2) {
        myNode* elem = &chn.begin()->cast<myNode>();
        ASSERT_FALSE(nul(elem));
        ASSERT_EQ(elem->number, val1);

        elem = &(++chn.begin())->cast<myNode>();
        ASSERT_FALSE(nul(elem));
        ASSERT_EQ(elem->number, val2);
    }

    struct myNode2 : public myNode {
        WRD(CLASS(myNode2, myNode))

    public:
        myNode2(int val) : super(val) {}
    };

    bool vectorHas(std::vector<int>& v, int val) {
        for(int n=0; n < v.size() ;n++)
            if(v[n] == val)
                return true;
        return false;
    }

    template <typename T = myNode>
    static wbool isMyNodesHasEqualIntArray(const tnchain<int, myNode>& root, int expects[], int expectSize) {
        auto myE = root.begin();
        vector<int> actuals;
        for(const auto& elem : root)
            if(elem.isSub<T>())
                actuals.push_back(elem.number);

        if(actuals.size() != expectSize) return false;
        for(int n=0; n < expectSize ;n++)
            if(!vectorHas(actuals, expects[n])) return false;
        return true;
    }

}

TEST(nchainTest, instantiateTest) {
    nchain chn;
    ASSERT_FALSE(nul(chn.getContainer()));
}

TEST(nchainTest, simpleAddDelTest10) {
    simpleAddDelTest(10);
}
TEST(nchainTest, simpleAddDelTest1000) {
    simpleAddDelTest(1000);
}
TEST(nchainTest, simpleAddDelTest10000) {
    simpleAddDelTest(10000);
}

TEST(nchainTest, testucontainableAPI) {
    //  initial state:
    tstr<nchain> arr(new nchain());
    bicontainable* con = &arr.get();
    ASSERT_EQ(con->len(), 0);

    auto head = arr->begin();
    ASSERT_TRUE(head.isEnd());
    auto tail = con->end();
    ASSERT_TRUE(tail.isEnd());

    ASSERT_TRUE(con->add("0", new myNode(0)));
    ASSERT_TRUE(con->add("1", new myMyNode(1)));
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);

    //  add:
    for(int n=0; n < con->len() ;n++)
        ASSERT_EQ(con->get(std::to_string(n)).cast<myNode>().number, n);

    //  get & each:
    {
        tnarr<myNode> tray = arr->getAll<myNode>([](const std::string& name, const myNode& elem) {
            return true;
        });
        ASSERT_EQ(tray.len(), 2);

        int cnt = 0;
        tray = arr->getAll<myNode>([&cnt](const std::string& name, const myNode& elem) {
            if(cnt >= 1) return false;
            cnt++;
            return true;
        });
        ASSERT_EQ(tray.len(), 1);
    }

    tnarr<myMyNode> tray = arr->getAll<myMyNode>([](const std::string& name, const myMyNode& elem) {
        if(elem.number == 1) return true;
        return false;
    });
    ASSERT_EQ(tray.len(), 1);

    //  del:
    ASSERT_TRUE(con->del("1"));
    ASSERT_EQ(con->len(), 1);
    ASSERT_EQ(con->begin()->cast<myNode>().number, 0);

    //  add with element:
    narr arr2;
    ASSERT_EQ(arr2.add(*con), 1);
    ASSERT_TRUE(arr2.add(new myNode(1)));
    ASSERT_TRUE(arr2.add(new myMyNode(2)));
    ASSERT_TRUE(arr2.add(new myNode(3)));
    ASSERT_EQ(arr2[2].cast<myNode>().number, 2);
    ASSERT_EQ(arr2[3].cast<myNode>().number, 3);
    ASSERT_EQ(arr2.len(), 4);

    auto e = arr2.begin();
    e = e + 2;
    ASSERT_EQ(e->cast<myNode>().number, 2);
    ASSERT_TRUE(arr2.add(e, new myNode(5)));
    ASSERT_TRUE(arr2.add(2, new myNode(6)));

    ASSERT_EQ(arr2[0].cast<myNode>().number, 0);
    ASSERT_EQ(arr2[1].cast<myNode>().number, 1);
    ASSERT_EQ(arr2[2].cast<myNode>().number, 6);
    ASSERT_EQ(arr2[3].cast<myNode>().number, 5);
    ASSERT_EQ(arr2[4].cast<myNode>().number, 2);
    ASSERT_EQ(arr2[5].cast<myNode>().number, 3);

    ASSERT_EQ(con->len(), 1);
    wcnt count = 0;
    for(auto e=arr2.iterate(1); e != arr2.iterate(3) ;++e)
        count += con->add(to_string(count), *e);
    ASSERT_EQ(count, 2);
    ASSERT_EQ(con->len(), 3);

    // con = {0, 1, 6}

    auto e2=con->begin();
    myNode* elem = &e2->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 0);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 1);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 6);
    ASSERT_FALSE(++e2);

    ASSERT_TRUE(con->len() > 0);
    con->rel();
    ASSERT_TRUE(con->len() == 0);

    count = 0;
    for(auto e=arr2.iterate(2); e ;++e)
        count += con->add(to_string(count), *e);
    ASSERT_EQ(count, 4);
    e2 = con->begin();
    elem = &e2->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 6);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 5);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 2);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 3);

    ASSERT_EQ(con->del(con->begin() + 1, con->begin() + 3), 2);
    e2 = con->begin();
    elem = &e2->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 6);

    elem = &(++e2)->cast<myNode>();
    ASSERT_FALSE(nul(elem));
    ASSERT_EQ(elem->number, 3);
}

TEST(nchainTest, testLinkedChainWithOnly1Element) {
    nmap map1;
    map1.add("0", new myNode(0));
    nchain chn1(map1);
    ASSERT_EQ(chn1.len(), 1);

    nmap map2;
    map2.add("1", new myNode(0));
    nchain chn2(map2);
    ASSERT_EQ(chn2.len(), 1);
    chn2.link(chn1);
    ASSERT_EQ(chn2.len(), 2);

    int n=0;
    for(auto e=chn2.begin(); e ;++e)
        n++;
    ASSERT_EQ(n, 2);
}

TEST(nchainTest, testLinkedChainWithNContainerAPI) {
    nmap map1;
    nchain chn1(map1);
    nmap map2;
    nchain chn2(map2);
    vector<int> expectElementNums;

    // add each chains:
    ASSERT_TRUE(chn2.add("6", new myNode(6)));
    ASSERT_TRUE(chn2.add("5", new myMyNode(5)));
    examineChain2Element(chn2, 6, 5);

    nmap arr3;
    nchain chn3(arr3);
    ASSERT_TRUE(chn3.add("2", new myNode(2)));
    ASSERT_TRUE(chn3.add("3", new myMyNode(3)));
    examineChain2Element(chn3, 2, 3);

    ASSERT_TRUE(map1.add("0", new myNode(0)));
    ASSERT_TRUE(chn1.add("1", new myMyNode(1)));
    examineChain2Element(chn1, 0, 1);

    expectElementNums.push_back(0);
    expectElementNums.push_back(1);
    expectElementNums.push_back(6);
    expectElementNums.push_back(5);
    expectElementNums.push_back(2);
    expectElementNums.push_back(3);

    // link:
    ASSERT_TRUE(nul(chn1.getNext()));
    ASSERT_TRUE(nul(chn2.getNext()));
    ASSERT_TRUE(nul(chn3.getNext()));
    ASSERT_TRUE(chn1.link(chn2));
    ASSERT_TRUE(nul(chn1.getNext().getNext()));
    ASSERT_TRUE(chn2.link(chn3));
    ASSERT_TRUE(nul(chn3.getNext()));
    ASSERT_TRUE(nul(chn1.getNext().getNext().getNext()));
    ASSERT_EQ(chn1.len(), 6);

    // add with link:
    ASSERT_EQ(chn1.get<myNode>("0").number, 0);
    ASSERT_EQ(chn1.get<myNode>("1").number, 1);
    ASSERT_EQ(chn1.get<myNode>("6").number, 6);
    ASSERT_EQ(chn1.get<myNode>("5").number, 5);
    ASSERT_EQ(chn1.get<myNode>("2").number, 2);
    ASSERT_EQ(chn1.get<myNode>("3").number, 3);

    // each with link:
    int cnt = 0;
    auto lambda = [&cnt, &expectElementNums](const nbicontainer& chn) -> void {
        for(const auto& e : chn) {
            const myNode& elem = e.cast<myNode>();
            if(nul(elem)) {
                cnt = -1;
                return;
            }

            if(elem.number != expectElementNums[cnt++]) {
                cnt = -1;
                return;
            }
        }
    };
    lambda(chn1);
    ASSERT_EQ(cnt, 6);

    cnt = 2;
    lambda(chn2);
    ASSERT_EQ(cnt, 6);

    cnt = 4;
    expectElementNums[5] = -1;
    lambda(chn3);
    ASSERT_EQ(cnt, -1);

    // del with link:
    ASSERT_EQ(chn1.len(), 6);
    ASSERT_EQ(chn1.getContainer().len(), 2);
    ASSERT_EQ(chn2.len(), 4);
    ASSERT_EQ(chn2.getContainer().len(), 2);
    ASSERT_EQ(chn3.len(), 2);
    ASSERT_EQ(chn3.getContainer().len(), 2);

    ASSERT_EQ(chn1.del(chn1.begin()+1, chn2.begin()+1), 2);
    ASSERT_EQ(chn1.len(), 4);
    ASSERT_EQ(chn2.len(), 3);
}

TEST(nchainTest, testIfnchainLinkItself) {
    nchain chn;
    chn.add("0", new myNode(0));
    chn.add("1", new myNode(1));
    ASSERT_FALSE(chn.link(chn));
    ASSERT_EQ(chn.len(), 2);
}

TEST(nchainTest, testShouldLinkOverwritePrevious) {
    tstr<nmap> map1Str(new nmap());
    const bindTag* map1tag = &bindTag::getBindTag(map1Str.getItsId());
    ASSERT_FALSE(nul(map1tag));
    ASSERT_EQ(map1tag->getStrongCnt(), 1);

    tweak<nmap> map1Weak = *map1Str;
    map1Str->add("0", new myNode(0));
    map1Str->add("1", new myNode(1));
    ASSERT_EQ(map1Str->len(), 2);
    ASSERT_EQ(map1tag->getStrongCnt(), 1);


    nchain chn2;
    chn2.add("2", new myNode(2));
    chn2.add("3", new myNode(3));
    ASSERT_EQ(chn2.len(), 2);

    ASSERT_TRUE(chn2.link(*map1Str));
    ASSERT_EQ(map1tag->getStrongCnt(), 2);
    // chn2 --> unknown chain instance holding map1
    ASSERT_EQ(chn2.len(), 4);

    map1Str.rel();
    ASSERT_EQ(map1tag->getStrongCnt(), 1);
    ASSERT_EQ(chn2.len(), 4);
    ASSERT_TRUE(map1Weak.isBind());

    nmap map2;
    ASSERT_TRUE(map1Weak.isBind());
    chn2.link(map2);
    ASSERT_EQ(map1tag->getStrongCnt(), 0);
    // this overwrites chain containing map1. it's now dangling.
    // chn2(2, 3) --> unknown chain instance holding map2(null)
    //   |--- X --> unknown chain instance holding map1(0, 1)
    ASSERT_FALSE(map1Weak.isBind());

    ASSERT_EQ(chn2.len(), 2);
}

TEST(nchainTest, testDelWithLink) {
    nchain chn;
    chn.add("1", new myNode(1));
    ASSERT_EQ(chn.len(), 1);

    {
        nmap map1;
        map1.add("2", new myNode(2));
        map1.add("3", new myNode(3));

        auto map1Str = chn.link(map1);
        // chn --> map1Str with map1
        //  ^
        //  |
        // head
        ASSERT_EQ(chn.len(), 3);
        tweak<nchain> map2Weak;
        {
            nmap map2;
            map2.add("4", new myNode(4));
            map2.add("5", new myNode(5));
            map2.add("6", new myNode(6));
            auto map2Str = map1Str->link(map2);
            map2Weak = map2Str;
            // now, chn --> map1Str with map1 --> map2Str with map2
            ASSERT_EQ(map2Str->len(), 3);
            ASSERT_EQ(map1Str->len(), 3 + map1.len());
            ASSERT_EQ(chn.len(), 1 + map1.len() + map2.len());
        }

        map1Str->unlink();
        // now, chn --> map1Str with map1
        ASSERT_FALSE(map2Weak.isBind());
        ASSERT_EQ(chn.len(), 1 + map1.len());

        WRD_DI("chn.len()=%d", chn.len());
        auto e = chn.iterate(chn.len() - 1);

        myNode& last = e->cast<myNode>();
        ASSERT_EQ(last.number, 3);
    }
    chn.unlink();
    ASSERT_EQ(chn.len(), 1);
}

TEST(nchainTest, testLastIterator) {
    nchain chn;
    ASSERT_EQ(chn.len(), 0);
    chn.add("1", new myNode(1));
    chn.add("3", new myNode(3));
    chn.add("4", new myNode(2));

    wbool sorted = true;
    ASSERT_TRUE(chn.has("1"));
    ASSERT_TRUE(chn.has("3"));
    ASSERT_TRUE(chn.has("4"));
    ASSERT_TRUE(sorted);
}

TEST(nchainTest, testRangeBasedForLoop) {
    nchain map1;
    map1.add("3", new myNode(3));
    map1.add("7", new myNode(7));
    nchain map2;
    map2.add("1", new myNode(1));
    map2.add("2", new myNode(2));
    map1.link(map2);

    int sum = 0;
    for(auto& e : map1) {
        myNode& cast = e.cast<myNode>();
        sum += cast.number;
    }

    int sum2 = 0;
    for(const node& e : map1) {
        const myNode& cast = e.cast<myNode>();
        sum2 += cast.number;
    }
    ASSERT_EQ(sum2, sum);

    int expect = 0;
    for(auto e=map1.begin(); e ; e++)
        expect += e->cast<myNode>().number;

    ASSERT_EQ(sum, expect);
}

TEST(nchainTest, testLinkArrayAndChain) {

    nmap map1;
    map1.add("1", new myNode(1));
    map1.add("2", new myNode(2));
    nchain chn(map1);
    ASSERT_EQ(chn.len(), 2);
    ASSERT_TRUE(nul(chn.getNext()));

    nchain chn2;
    chn2.add("3", new myNode(3));
    nmap map2;
    map2.add("4", new myNode(4));
    map2.add("5", new myNode(5));
    chn2.link(map2);
    ASSERT_EQ(chn2.len(), 3);
    ASSERT_FALSE(nul(chn2.getNext()));
    ASSERT_EQ(&chn2.getNext().getContainer(), &map2);

    chn.link(chn2);
    int cnt = 5;
    ASSERT_EQ(chn.len(), cnt);

    for(int n=0; n < 5; n++) {
        std::string key = std::to_string(n);
        ASSERT_TRUE(chn.has(key));
        ASSERT_EQ(chn[key].cast<myNode>().number, n);
    }
}

TEST(nchainTest, testDeepChainIteration) {
    tnchain<int, myNode> chn1;
    chn1.add(1, new myNode(1));
    chn1.add(2, new myNode(2));
    ASSERT_EQ(chn1.len(), 2);

    tnchain<int, myNode> chn2;
    chn2.add(3, new myNode(3));
    chn2.add(4, new myNode(4));
    ASSERT_EQ(chn2.len(), 2);

    tstr<tnchain<int, myNode>> root(tnchain<int, myNode>::wrapDeep(chn1));
    ASSERT_EQ(root->len(), 2);

    chn1.link(chn2);
    ASSERT_EQ(chn1.len(), 4);
    ASSERT_EQ(root->len(), 4);

    tnchain<int, myNode> chn3;
    chn3.add(5, new myNode(5));
    chn3.add(6, new myNode(6));
    ASSERT_EQ(chn3.len(), 2);

    root->link(chn3);
    ASSERT_EQ(root->len(), 6);

    int expect = 1;
    std::vector<int> tray;
    for(auto e=root->begin(); e ;++e) {
        ASSERT_EQ(e.getKey(), e->cast<myNode>().number);
        tray.push_back(e.getKey());
    }

    for(int n=0; n < 6; n++)
        ASSERT_TRUE(vectorHas(tray, n));
}

TEST(nchainTest, testDeepChainAddDel) {
    tnchain<int, myNode> chn1;
    chn1.add(1, new myNode(1));
    chn1.add(2, new myNode2(2));

    tnchain<int, myNode> chn2;
    chn2.add(3, new myNode2(3));
    chn2.add(4, new myNode(4));
    chn1.link(chn2);

    tstr<tnchain<int, myNode>> root(tnchain<int, myNode>::wrapDeep(chn1));

    tnchain<int, myNode> chn3;
    chn3.add(5, new myNode2(5));
    chn3.add(6, new myNode(6));

    root->link(chn3);

    auto e = root->begin();
    e = e + 2;
    ASSERT_FALSE(nul(*e));

    ASSERT_EQ(root->get(6).number, 6);
    root->del(3);
    ASSERT_EQ(root->len(), 5);

    {
        int expects[] = {1, 2, 4, 5, 6};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<>(*root, expects, 5));
    }

    {
        int expects[] = {2, 5};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<myNode2>(*root, expects, 2));
    }

    ASSERT_EQ(root->len(), 5);
    root->del(root->begin() + 1, root->last());
    ASSERT_EQ(root->len(), 2);

    {
        int expects[] = {1, 6};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<>(*root, expects, 2));
    }

    bool found = false;
    for(const auto& elem : *root)
        if(elem.isSub<myNode2>()) {
            found = true;
            break;
        }
    ASSERT_FALSE(found);
}

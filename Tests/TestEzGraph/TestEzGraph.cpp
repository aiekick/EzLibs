#include <TestEzGraph.h>
#include <EzGraph/EzGraph.hpp>

using namespace ez;

class NodeNumber;
typedef std::shared_ptr<NodeNumber> NodeNumberPtr;
typedef std::weak_ptr<NodeNumber> NodeNumberWeak;

class NodeNumber : public Node {
public:
    static NodeNumberPtr create(const NodeDatas& vNodeDatas) {
        auto ret = std::make_shared<NodeNumber>();
        ret->m_setThis(ret);
        if (!ret->init(vNodeDatas)) { ret.reset(); }
        return ret;
    }

private:
    float m_Value = 0.0f;

public:
    void setValue(const float vValue) { m_Value = vValue; }
    float getValue() { return m_Value; }
};

class NodeOpAdd;
typedef std::shared_ptr<NodeOpAdd> NNodeOpAddPtr;
typedef std::weak_ptr<NodeOpAdd> NodeOpAddWeak;

class NodeOpAdd : public NodeNumber {
public:
    static NNodeOpAddPtr create(const NodeDatas& vNodeDatas) {
        auto ret = std::make_shared<NodeOpAdd>();
        ret->m_setThis(ret);
        if (!ret->init(vNodeDatas)) { ret.reset(); }
        return ret;
    }

    RetCodes eval(const size_t vFrame, const SlotWeak& vFrom) final {
        auto ret = RetCodes::SUCCESS;
        float sum = 0.0f;
        auto outSlotPtr = vFrom.lock();
        if (outSlotPtr != nullptr) {
            auto outNodePtr = std::dynamic_pointer_cast<NodeOpAdd>(outSlotPtr->getParentNode().lock());
            if (outNodePtr != nullptr) {
                for (auto inSlotPtr : m_getInputsRef()) {
                    if (inSlotPtr != nullptr) {
                        for (const auto& conSlot : inSlotPtr->getConnectedSlots()) {
                            auto conSlotPtr = conSlot.lock();
                            if (conSlotPtr != nullptr) {
                                auto inNodePtr = std::dynamic_pointer_cast<NodeNumber>(conSlotPtr->getParentNode().lock());
                                if (inNodePtr != nullptr) {
                                    sum += inNodePtr->getValue();
                                } else {
                                    ret = RetCodes::FAILED_SLOT_PTR_NULL;
                                    break;
                                }
                            }
                        }
                    }
                }
                outNodePtr->setValue(sum);
                EvalDatas datas;
                datas.frame = vFrame;
                outSlotPtr->setLastEvaluatedDatas(datas);
            } else {
                ret = RetCodes::FAILED_SLOT_PTR_NULL;
            }
        } else {
            ret = RetCodes::FAILED_SLOT_PTR_NULL;
        }
        return ret;
    }
};

static SlotPtr createOutputSlot() {
    SlotDatas datas;
    datas.m_Dir = SlotDir::OUTPUT;
    return Slot::create(datas);
}

static SlotPtr createInputSlot() {
    SlotDatas datas;
    datas.m_Dir = SlotDir::INPUT;
    return Slot::create(datas);
}

static GraphPtr createGraph() {
    GraphDatas datas;
    return Graph::create(datas);
}

////////////////////////////////////////////////////////////////////////////
////  Graph ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_Graph_Building_0() { return true; }

bool Test_Graph_Evaluation_0() {
    NodeDatas datas;
    auto graphPtr = createGraph();
    if (graphPtr == nullptr) { return false; }
    auto nodeNumASlotOutputPtr = createOutputSlot();
    if (nodeNumASlotOutputPtr == nullptr) { return false; }
    auto nodaNumAPtr = NodeNumber::create(datas);
    if (nodaNumAPtr == nullptr) { return false; }
    if (graphPtr->addNode(nodaNumAPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaNumAPtr->addSlot(nodeNumASlotOutputPtr) != RetCodes::SUCCESS) { return false; }
    nodaNumAPtr->setValue(5.0f);
    if (nodaNumAPtr->getValue() != 5.0f) { return false; }
    auto nodeNumBSlotOutputPtr = createOutputSlot();
    if (nodeNumBSlotOutputPtr == nullptr) { return false; }
    auto nodaNumBPtr = NodeNumber::create(datas);
    if (nodaNumBPtr == nullptr) { return false; }
    if (graphPtr->addNode(nodaNumBPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaNumBPtr->addSlot(nodeNumBSlotOutputPtr) != RetCodes::SUCCESS) { return false; }
    nodaNumBPtr->setValue(200.0f);
    if (nodaNumBPtr->getValue() != 200.0f) { return false; }
    auto nodeOpAddSlotInputAPtr = createInputSlot();
    if (nodeOpAddSlotInputAPtr == nullptr) { return false; }
    auto nodeOpAddSlotInputBPtr = createInputSlot();
    if (nodeOpAddSlotInputBPtr == nullptr) { return false; }
    auto nodeOpAddSlotOutputPtr = createOutputSlot();
    if (nodeOpAddSlotOutputPtr == nullptr) { return false; }
    auto nodaOpAddPtr = NodeOpAdd::create(datas);
    if (graphPtr->addNode(nodaOpAddPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaOpAddPtr == nullptr) { return false; }
    if (nodaOpAddPtr->addSlot(nodeOpAddSlotInputAPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaOpAddPtr->addSlot(nodeOpAddSlotInputBPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaOpAddPtr->addSlot(nodeOpAddSlotOutputPtr) != RetCodes::SUCCESS) { return false; }
    if (graphPtr->connectSlots(nodeNumASlotOutputPtr, nodeOpAddSlotInputAPtr) != RetCodes::SUCCESS) { return false; }
    if (graphPtr->connectSlots(nodeNumBSlotOutputPtr, nodeOpAddSlotInputBPtr) != RetCodes::SUCCESS) { return false; }
    if (nodaNumAPtr != nullptr && nodaNumBPtr != nullptr && nodaOpAddPtr != nullptr) {
        if (nodaOpAddPtr->eval(10, nodeOpAddSlotOutputPtr) != RetCodes::SUCCESS) {
            return false;
        } else {
            if (nodaOpAddPtr->getValue() != 205.0f) { return false; }
            if (nodeOpAddSlotOutputPtr->getLastEvaluatedDatas().frame != 10) { return false; }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

#define IfTestCollectionExist(v, str) \
    if (vTest.find(#v) != std::string::npos) return v(str)

bool Test_Graph(const std::string& vTest) {
    IfTestExist(Test_Graph_Building_0);
    else IfTestExist(Test_Graph_Evaluation_0);
    return true;
}
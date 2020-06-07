using namespace std;

enum Status {
    Free,
    Split,
    Occupied
};

class Block {
    static const size_t minSize = 32;

private:
    Block* leftChild;
    Block* rightChild;
    size_t size;
    Status status;
    int position;
    int* ptr;

    void splitBlock() {
        status = Split;
        leftChild = new Block(position, size / 2);
        rightChild = new Block(position + size / 2, size / 2);
    }

    void tryMerge() {
        if (rightChild->getStatus() == Free && leftChild->getStatus() == Free)
        {
            int leftPos = leftChild->getPosition();
            int rightPos = rightChild->getPosition();

            cout << "[DEBUG] Blocks [" << leftPos << ":" << leftPos + size / 2 - 1 << "] and [" <<
                 rightPos << ":" << rightPos + size / 2 - 1 << "] were merged." << endl;

            free(leftChild);
            free(rightChild);
            status = Free;
        }
    }

    int getPosition() {
        return position;
    }

    void allocate(size_t content_size) {
        status = Occupied;
        ptr = (int*)malloc(content_size);

        cout << "[DEBUG] " << content_size << " bytes added to block [" << position << ":" << position + size - 1 << "]." << endl;
    }

public:
    Block(int blockStartPosition, size_t size) {
        position = blockStartPosition;
        this->size = size;
        status = Free;
    }

    ~Block() {
        free(leftChild);
        free(rightChild);
        free(ptr);
    }

    int* fillBlock(size_t contentSize) {
        if (status == Occupied) {
            return nullptr;
        }

        if (status == Free) {
            if (size <= minSize && contentSize <= size) {
                allocate(contentSize);
                return ptr;
            }

            if (contentSize <= size / 2) {
                splitBlock();
                return leftChild->fillBlock(contentSize);
            }
            else {
                allocate(contentSize);
                return ptr;
            }
        }

        if (status == Split) {
            int* res = leftChild->fillBlock(contentSize);
            if (res == nullptr) {
                return rightChild->fillBlock(contentSize);
            }
            return res;
        }
    }

    bool tryReleaseData(int* dataPtr) {
        if (status == Free)
            return false;

        if (status == Occupied) {
            if (ptr == dataPtr) {
                free(ptr);
                status = Free;

                cout << "[DEBUG] Data was released from block [" << position << ":" << position + size - 1 << "]." << endl;

                return true;
            }
            return false;
        }

        if (status == Split) {
            if (leftChild->tryReleaseData(dataPtr) || rightChild->tryReleaseData(dataPtr)) {
                tryMerge();
                return true;
            }
        }

        return false;
    }

    Status getStatus() {
        return status;
    }
};
namespace eurora::core {

template <typename T>
class NDArrayFactory {
public:
    static std::shared_ptr<MathBase<T>> Create(const std::string& backend) {
        if (backend == "Eigen") {
            return std::make_shared<MathEigen<T>>();
        }
        if (backend == "XTensor") {
            return std::make_shared<MathXTensor<T>>();
        }
        if (backend == "CUDA") {
            return std::make_shared<MathCuda<T>>();
        }
        throw std::runtime_error("Unsupported backend: " + backend);
    }
};

}  // namespace eurora::core

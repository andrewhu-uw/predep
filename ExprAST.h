namespace dep
{
    /// Base class for all expression nodes.
    class ExprAST {
    public:
        virtual ~ExprAST() {}
    };

    /// Expression class for floating point literals like "1.0".
    class FloatExprAST : public ExprAST {
        double Val;

    public:
        FloatExprAST(double Val) : Val(Val) {}
    };

    /// Expression class for floating point literals like "1.0".
    class IntExprAST : public ExprAST {
        long Val;

    public:
        IntExprAST(long Val) : Val(Val) {}
    };

    /// Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST {
        std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
    };

    /// Expression class for a binary operator.
    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
            std::unique_ptr<ExprAST> RHS)
            : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

    /// Expression class for function calls.
    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string &Callee,
            std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}
    };
}
namespace Crust {
class SourceLocation {
   public:
    SourceLocation() : mCurrentLine{0}, mCurrentColumn{0} {}

    SourceLocation(const unsigned line, const unsigned column) : mCurrentLine{line}, mCurrentColumn{column} {}

    void init();
    void advance(bool nextLine = false);
    const unsigned getCurrentColumn() const { return mCurrentColumn; }
    const unsigned getCurrentLine() const { return mCurrentLine; }

   private:
    unsigned mCurrentLine;   /*!< Current line in the file the lexer is at */
    unsigned mCurrentColumn; /*!< Current column in the file the lexer is at */
};
}  // namespace Crust
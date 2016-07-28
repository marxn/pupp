#include "parser/operation.h"

using namespace std;

ConstValue * Operation::AddOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;
        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() + static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new DecimalValue(leftvalue + *static_cast<DecimalValue*>(right));
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new DecimalValue(*static_cast<DecimalValue*>(left) + rightvalue);
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                int prec = value1->GetPrec();

                if(static_cast<DecimalValue*>(right)->GetPrec() > prec)
                {
                        prec = static_cast<DecimalValue*>(right)->GetPrec();
                }

                result = new DecimalValue(*value1 + *value2);        
                static_cast<DecimalValue*>(result)->SetPrec(prec);
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new IntegerValue(value1->GetValue() + value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 + value2);
                        static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new IntegerValue(value1.GetValue() + value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new DecimalValue(value1 + value2);
                        static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new DecimalValue(*value1 + value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        int prec = value1->GetPrec();

                        if(static_cast<StringValue*>(right)->GetNumPrec() > prec)
                        {
                                prec = static_cast<StringValue*>(right)->GetNumPrec();
                        }

                        DecimalValue value2(right->toString());
                        result = new DecimalValue(*value1 + value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new DecimalValue(value1 + *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());

                        int prec = leftvalue->GetNumPrec();

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        if(value2->GetPrec() > prec)
                        {
                                prec = value2->GetPrec();
                        }

                        result = new DecimalValue(value1 + *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                result = new StringValue(left->toString() + right->toString());
        }
        else
        {
                result = new NullValue;
        }
        return result;
}

ConstValue * Operation::SubOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() - static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new DecimalValue(leftvalue - *static_cast<DecimalValue*>(right));
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new DecimalValue(*static_cast<DecimalValue*>(left) - rightvalue);
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                int prec = value1->GetPrec();

                if(static_cast<DecimalValue*>(right)->GetPrec() > prec)
                {
                        prec = static_cast<DecimalValue*>(right)->GetPrec();
                }

                result = new DecimalValue(*value1 - *value2);        
                static_cast<DecimalValue*>(result)->SetPrec(prec);
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new IntegerValue(value1->GetValue() - value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new IntegerValue(value1.GetValue() - value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new DecimalValue(value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new DecimalValue(*value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        int prec = value1->GetPrec();

                        if(static_cast<StringValue*>(right)->GetNumPrec() > prec)
                        {
                                prec = static_cast<StringValue*>(right)->GetNumPrec();
                        }

                        DecimalValue value2(right->toString());
                        result = new DecimalValue(*value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new DecimalValue(value1 - *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());

                        int prec = leftvalue->GetNumPrec();

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        if(value2->GetPrec() > prec)
                        {
                                prec = value2->GetPrec();
                        }

                        result = new DecimalValue(value1 - *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue value2(rightvalue->toString());
                        result = new IntegerValue(value1.GetValue() - value2.GetValue());
                }
                else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                }
                else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 - value2);
                        static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                }
                else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 - value2);

                        int prec = leftvalue->GetNumPrec();
                        if(rightvalue->GetNumPrec() > prec)
                        {
                                prec = rightvalue->GetNumPrec();
                        }
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else
        {
                result = new NullValue;
        }

        return result;
}

ConstValue * Operation::MulOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new IntegerValue(static_cast<IntegerValue*>(left)->GetValue() * static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new DecimalValue(leftvalue * *static_cast<DecimalValue*>(right));
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(right)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new DecimalValue(*static_cast<DecimalValue*>(left) * rightvalue);
                static_cast<DecimalValue*>(result)->SetPrec(static_cast<DecimalValue*>(left)->GetPrec());
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                int prec = value1->GetPrec() + value2->GetPrec();

                result = new DecimalValue(*value1 * *value2);        
                static_cast<DecimalValue*>(result)->SetPrec(prec);
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new IntegerValue(value1->GetValue() * value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new IntegerValue(value1.GetValue() * value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new DecimalValue(value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new DecimalValue(*value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value1->GetPrec());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        int prec = value1->GetPrec() + static_cast<StringValue*>(right)->GetNumPrec();

                        DecimalValue value2(right->toString());
                        result = new DecimalValue(*value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = left->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new DecimalValue(value1 * *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(value2->GetPrec());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        int prec = leftvalue->GetNumPrec() + value2->GetPrec();

                        result = new DecimalValue(value1 * *value2);
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = right->DupValue();
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue value2(rightvalue->toString());
                        result = new IntegerValue(value1.GetValue() * value2.GetValue());
                }
                else if(leftvalue->LooksLikeInteger() && rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(rightvalue->GetNumPrec());
                }
                else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeInteger())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 * value2);
                        static_cast<DecimalValue*>(result)->SetPrec(leftvalue->GetNumPrec());
                }
                else if(leftvalue->LooksLikeDecimal() && rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(rightvalue->toString());

                        result = new DecimalValue(value1 * value2);

                        int prec = leftvalue->GetNumPrec() + rightvalue->GetNumPrec();
                        static_cast<DecimalValue*>(result)->SetPrec(prec);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else
        {
                result = new NullValue;
        }

        return result;

}

ConstValue * Operation::DivOperation(ConstValue * left, ConstValue * right)
{
        DecimalValue * result = NULL;

        DecimalValue value1(left->toString());
        DecimalValue value2(right->toString());

        result = new DecimalValue(value1 / value2);
        result->SetPrec(15);

        return result;
}

ConstValue * Operation::GTOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() > static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue > *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) > rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 > *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() > value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 > value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() > value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 > value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 > value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 > value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 > *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 > *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString().compare(rightvalue->toString()) > 0);
        }
        else
        {
                result = new NullValue;
        }

        return result;
}

ConstValue * Operation::LTOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() < static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue < *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) < rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 < *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() < value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 < value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() < value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 < value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 < value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 < value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 < *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 < *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString().compare(rightvalue->toString()) < 0);
        }
        else
        {
                result = new NullValue;
        }


        return result;
}

ConstValue * Operation::EQOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() == static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue == *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) == rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 == *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() == value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 == value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() == value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 == value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 == value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 == value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 == *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 == *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString() == rightvalue->toString());
        }
        else if(left->GetType()==Boolean && right->GetType()==Boolean)
        {
                result = new BooleanValue(static_cast<BooleanValue*>(left)->GetValue() == static_cast<BooleanValue*>(right)->GetValue());
        }
        else
        {
                result = new NullValue;
        }


        return result;
}

ConstValue * Operation::NEQOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() != static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue != *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) != rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 != *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() != value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 != value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() != value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 != value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 != value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 != value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 != *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 != *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString() != rightvalue->toString());
        }
        else
        {
                result = new NullValue;
        }

        return result;
}

ConstValue * Operation::GEOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() >= static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue >= *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) >= rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 >= *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() >= value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 >= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() >= value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 >= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 >= value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 >= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 >= *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 >= *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString().compare(rightvalue->toString()) >= 0);
        }
        else
        {
                result = new NullValue;
        }


        return result;
}

ConstValue * Operation::LEOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Integer && right->GetType()==Integer)
        {
                result = new BooleanValue(static_cast<IntegerValue*>(left)->GetValue() <= static_cast<IntegerValue*>(right)->GetValue());
        }
        else if(left->GetType()==Integer && right->GetType()==Decimal)
        {
                DecimalValue leftvalue(static_cast<IntegerValue*>(left));
                result = new BooleanValue(leftvalue <= *static_cast<DecimalValue*>(right));
        }
        else if(left->GetType()==Decimal && right->GetType()==Integer)
        {
                DecimalValue rightvalue(static_cast<IntegerValue*>(right));
                result = new BooleanValue(*static_cast<DecimalValue*>(left) <= rightvalue);
        }
        else if(left->GetType()==Decimal && right->GetType()==Decimal)
        {
                DecimalValue * value1 = static_cast<DecimalValue*>(left);
                DecimalValue * value2 = static_cast<DecimalValue*>(right);

                result = new BooleanValue(*value1 <= *value2);        
        }
        else if(left->GetType()==Integer && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        IntegerValue * value1 = static_cast<IntegerValue*>(left);
                        IntegerValue value2(rightvalue->toString());
                        result = new BooleanValue(value1->GetValue() <= value2.GetValue());
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(static_cast<IntegerValue*>(left));
                        DecimalValue value2(rightvalue->toString());

                        result = new BooleanValue(value1 <= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Integer)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                if(leftvalue->LooksLikeInteger())
                {
                        IntegerValue value1(leftvalue->toString());
                        IntegerValue * value2 = static_cast<IntegerValue*>(right);
                        result = new BooleanValue(value1.GetValue() <= value2->GetValue());
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue value2(static_cast<IntegerValue*>(right));

                        result = new BooleanValue(value1 <= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==Decimal && right->GetType()==String)
        {
                StringValue * rightvalue = static_cast<StringValue*>(right);

                if(rightvalue->LooksLikeInteger())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 <= value2);
                }
                else if(rightvalue->LooksLikeDecimal())
                {
                        DecimalValue * value1 = static_cast<DecimalValue*>(left);
                        DecimalValue value2(right->toString());

                        result = new BooleanValue(*value1 <= value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==Decimal)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);

                if(leftvalue->LooksLikeInteger())
                {
                        DecimalValue value1(left->toString());

                        DecimalValue * value2 = static_cast<DecimalValue*>(right);
                        result = new BooleanValue(value1 <= *value2);
                }
                else if(leftvalue->LooksLikeDecimal())
                {
                        DecimalValue value1(leftvalue->toString());
                        DecimalValue * value2 = static_cast<DecimalValue*>(right);

                        result = new BooleanValue(value1 <= *value2);
                }
                else
                {
                        result = new NullValue;
                }
        }
        else if(left->GetType()==String && right->GetType()==String)
        {
                StringValue * leftvalue = static_cast<StringValue*>(left);
                StringValue * rightvalue = static_cast<StringValue*>(right);

                result = new BooleanValue(leftvalue->toString().compare(rightvalue->toString()) <= 0);
        }
        else
        {
                result = new NullValue;
        }


        return result;
}

ConstValue * Operation::ANDOperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Boolean && right->GetType()==Boolean)
        {
                result = new BooleanValue(static_cast<BooleanValue*>(left)->GetValue() && static_cast<BooleanValue*>(right)->GetValue());
        }
        else
        {
                result = new NullValue;
        }
        return result;
}

ConstValue * Operation::OROperation(ConstValue * left, ConstValue * right)
{
        ConstValue * result = NULL;

        if(left->GetType()==Boolean && right->GetType()==Boolean)
        {
                result = new BooleanValue(static_cast<BooleanValue*>(left)->GetValue() || static_cast<BooleanValue*>(right)->GetValue());
        }
        else
        {
                result = new NullValue;
        }
        return result;
}

ConstValue * Operation::NOTOperation(ConstValue * value)
{
        ConstValue * result = NULL;

        if(value->GetType()==Boolean)
        {
                result = new BooleanValue(!static_cast<BooleanValue*>(value)->GetValue());
        }
        else
        {
                result = new NullValue;
        }
        return result;
}

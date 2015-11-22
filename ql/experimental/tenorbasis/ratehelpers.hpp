/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 StatPro Italia srl
 Copyright (C) 2007, 2008, 2009, 2015 Ferdinando Ametrano
 Copyright (C) 2007, 2009 Roland Lichters
 Copyright (C) 2015 Maddalena Zanzi
 Copyright (C) 2015 Paolo Mazzocchi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file ForwardHelpers.hpp
    \brief deposit, FRA, futures, and various swap rate helpers
*/

#ifndef quantlib_ForwardHelpers_hpp
#define quantlib_ForwardHelpers_hpp

#include <ql/termstructures/bootstraphelper.hpp>
#include <ql/instruments/vanillaswap.hpp>
#include <ql/instruments/bmaswap.hpp>
#include <ql/instruments/futures.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>

namespace QuantLib {

    class SwapIndex;
    class Quote;

    typedef BootstrapHelper<ForwardRateCurve> ForwardHelper;
    typedef RelativeDateBootstrapHelper<ForwardRateCurve>
                                                        RelativeDateForwardHelper;

    //! Rate helper for bootstrapping over IborIndex futures prices
    class FuturesForwardHelper : public ForwardHelper {
      public:
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          Natural lengthInMonths,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const Date& iborEndDate,
                          const DayCounter& dayCounter,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          const Date& endDate,
                          const DayCounter& dayCounter,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(const Handle<Quote>& price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<IborIndex>& iborIndex,
                          const Handle<Quote>& convexityAdjustment = Handle<Quote>(),
                          Futures::Type type = Futures::IMM);
        FuturesForwardHelper(Real price,
                          const Date& iborStartDate,
                          const boost::shared_ptr<IborIndex>& iborIndex,
                          Rate convexityAdjustment = 0.0,
                          Futures::Type type = Futures::IMM);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        //@}
        //! \name FuturesForwardHelper inspectors
        //@{
        Real convexityAdjustment() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        Time yearFraction_;
        Handle<Quote> convAdj_;
    };


    //! Rate helper for bootstrapping over deposit rates
    class DepositForwardHelper : public RelativeDateForwardHelper {
      public:
        DepositForwardHelper(const Handle<Quote>& rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositForwardHelper(Rate rate,
                          const Period& tenor,
                          Natural fixingDays,
                          const Calendar& calendar,
                          BusinessDayConvention convention,
                          bool endOfMonth,
                          const DayCounter& dayCounter);
        DepositForwardHelper(const Handle<Quote>& rate,
                          const boost::shared_ptr<IborIndex>& iborIndex);
        DepositForwardHelper(Rate rate,
                          const boost::shared_ptr<IborIndex>& iborIndex);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        boost::shared_ptr<IborIndex> iborIndex_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over %FRA rates
    class FraForwardHelper : public RelativeDateForwardHelper {
      public:
        FraForwardHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Natural monthsToStart,
                      Natural monthsToEnd,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Natural monthsToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Period periodToStart,
                      Natural lengthInMonths,
                      Natural fixingDays,
                      const Calendar& calendar,
                      BusinessDayConvention convention,
                      bool endOfMonth,
                      const DayCounter& dayCounter,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(const Handle<Quote>& rate,
                      Period periodToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        FraForwardHelper(Rate rate,
                      Period periodToStart,
                      const boost::shared_ptr<IborIndex>& iborIndex,
                      Pillar::Choice pillar = Pillar::LastRelevantDate,
                      Date customPillarDate = Date());
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      private:
        void initializeDates();
        Date fixingDate_;
        Period periodToStart_;
        Pillar::Choice pillarChoice_;
        boost::shared_ptr<IborIndex> iborIndex_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over swap rates
    /*! \todo use input SwapIndex to create the swap */
    class SwapForwardHelper : public RelativeDateForwardHelper {
      public:
        SwapForwardHelper(const Handle<Quote>& rate,
                       const boost::shared_ptr<SwapIndex>& swapIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<ForwardRateCurve>& discountingCurve
                                               = Handle<ForwardRateCurve>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapForwardHelper(const Handle<Quote>& rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<IborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<ForwardRateCurve>& discountingCurve
                                            = Handle<ForwardRateCurve>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapForwardHelper(Rate rate,
                       const boost::shared_ptr<SwapIndex>& swapIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<ForwardRateCurve>& discountingCurve
                                            = Handle<ForwardRateCurve>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        SwapForwardHelper(Rate rate,
                       const Period& tenor,
                       const Calendar& calendar,
                       // fixed leg
                       Frequency fixedFrequency,
                       BusinessDayConvention fixedConvention,
                       const DayCounter& fixedDayCount,
                       // floating leg
                       const boost::shared_ptr<IborIndex>& iborIndex,
                       const Handle<Quote>& spread = Handle<Quote>(),
                       const Period& fwdStart = 0*Days,
                       // exogenous discounting curve
                       const Handle<ForwardRateCurve>& discountingCurve
                                            = Handle<ForwardRateCurve>(),
                       Natural settlementDays = Null<Natural>(),
                       Pillar::Choice pillar = Pillar::LastRelevantDate,
                       Date customPillarDate = Date());
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name SwapForwardHelper inspectors
        //@{
        Spread spread() const;
        boost::shared_ptr<VanillaSwap> swap() const;
        const Period& forwardStart() const;
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
      protected:
        void initializeDates();
        Natural settlementDays_;
        Period tenor_;
        Pillar::Choice pillarChoice_;
        Calendar calendar_;
        BusinessDayConvention fixedConvention_;
        Frequency fixedFrequency_;
        DayCounter fixedDayCount_;
        boost::shared_ptr<IborIndex> iborIndex_;
        boost::shared_ptr<VanillaSwap> swap_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
        Handle<Quote> spread_;
        Period fwdStart_;
        Handle<ForwardRateCurve> discountHandle_;
        RelinkableHandle<ForwardRateCurve> discountRelinkableHandle_;
    };


    //! Rate helper for bootstrapping over BMA swap rates
    class BMASwapForwardHelper : public RelativeDateForwardHelper {
      public:
        BMASwapForwardHelper(const Handle<Quote>& liborFraction,
                          const Period& tenor, // swap maturity
                          Natural settlementDays,
                          const Calendar& calendar,
                          // bma leg
                          const Period& bmaPeriod,
                          BusinessDayConvention bmaConvention,
                          const DayCounter& bmaDayCount,
                          const boost::shared_ptr<BMAIndex>& bmaIndex,
                          // ibor leg
                          const boost::shared_ptr<IborIndex>& index);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
    protected:
        void initializeDates();
        Period tenor_;
        Natural settlementDays_;
        Calendar calendar_;
        Period bmaPeriod_;
        BusinessDayConvention bmaConvention_;
        DayCounter bmaDayCount_;
        boost::shared_ptr<BMAIndex> bmaIndex_;
        boost::shared_ptr<IborIndex> iborIndex_;

        boost::shared_ptr<BMASwap> swap_;
        RelinkableHandle<ForwardRateCurve> termStructureHandle_;
    };


    //! Rate helper for bootstrapping over Fx Swap rates
    /*! fwdFx = spotFx + fwdPoint
        isFxBaseCurrencyCollateralCurrency indicates if the base currency
        of the fx currency pair is the one used as collateral 
    */
    class FxSwapForwardHelper : public RelativeDateForwardHelper {
      public:
        FxSwapForwardHelper(const Handle<Quote>& fwdPoint,
                         const Handle<Quote>& spotFx,
                         const Period& tenor,
                         Natural fixingDays,
                         const Calendar& calendar,
                         BusinessDayConvention convention,
                         bool endOfMonth,
                         bool isFxBaseCurrencyCollateralCurrency,                   
                         const Handle<ForwardRateCurve>& collateralCurve);
        //! \name ForwardHelper interface
        //@{
        Real impliedQuote() const;
        void setTermStructure(ForwardRateCurve*);
        //@}
        //! \name FxSwapForwardHelper inspectors
        //@{
        Real spot() const { return spot_->value(); }
        Period tenor() const { return tenor_; }
        Natural fixingDays() const { return fixingDays_; }
        Calendar calendar() const { return cal_; }
        BusinessDayConvention businessDayConvention() const { return conv_; }
        bool endOfMonth() const { return eom_; }
        bool isFxBaseCurrencyCollateralCurrency() const {
                                return isFxBaseCurrencyCollateralCurrency_; }
        //@}
        //! \name Visitability
        //@{
        void accept(AcyclicVisitor&);
        //@}
    private:
        void initializeDates();
        Handle<Quote> spot_;
        Period tenor_;
        Natural fixingDays_;
        Calendar cal_;
        BusinessDayConvention conv_;
        bool eom_;
        bool isFxBaseCurrencyCollateralCurrency_;

        RelinkableHandle<ForwardRateCurve> termStructureHandle_;

        Handle<ForwardRateCurve> collHandle_;
        RelinkableHandle<ForwardRateCurve> collRelinkableHandle_;
    };



    // inline

    inline Spread SwapForwardHelper::spread() const {
        return spread_.empty() ? 0.0 : spread_->value();
    }

    inline boost::shared_ptr<VanillaSwap> SwapForwardHelper::swap() const {
        return swap_;
    }

    inline const Period& SwapForwardHelper::forwardStart() const {
        return fwdStart_;
    }

}

#endif
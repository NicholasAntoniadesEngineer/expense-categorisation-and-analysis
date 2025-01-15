"""
personal-finance/result_saver.py

This module contains the ResultSaver class which saves financial data
to CSV files based on specified options.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

import os
import pandas as pd

class ResultSaver:
    def __init__(self, output_directory, save_monthly, save_weekly, save_entire):
        self.output_directory = output_directory
        self.save_monthly = save_monthly
        self.save_weekly = save_weekly
        self.save_entire = save_entire

    def save_results(self, detailed_expenses):
        """
        Save the results to CSV files based on the save options.
        """
        if self.save_entire:
            output_filename = os.path.join(self.output_directory, f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv")
            detailed_expenses['Amount'] = pd.to_numeric(detailed_expenses['Amount'], errors='coerce')
            detailed_expenses.to_csv(output_filename, index=False)
            print(f"Results have been saved to {output_filename}")

        if self.save_monthly:
            self.save_monthly_summary(detailed_expenses)

        if self.save_weekly:
            self.save_weekly_summary(detailed_expenses)

    def save_monthly_summary(self, detailed_expenses):
        """
        Save monthly summary reports.
        """
        detailed_expenses['Month'] = detailed_expenses['Date'].dt.to_period('M')
        unique_months = detailed_expenses['Month'].unique()

        for month in unique_months:
            monthly_data = detailed_expenses[detailed_expenses['Month'] == month]
            monthly_summary = pd.pivot_table(
                monthly_data,
                values='Amount',
                index=['Month'],
                columns=['Category'],
                aggfunc='sum'
            ).reset_index().melt(
                id_vars=['Month'],
                var_name='Category',
                value_name='Amount'
            ).dropna()

            monthly_summary = monthly_summary[(monthly_summary['Category'] != 'Nothing') & (monthly_summary['Amount'] != 0)]
            total_amount = monthly_summary['Amount'].astype(float).sum()
            total_row = pd.DataFrame({'Month': [month], 'Category': ['Total'], 'Amount': [f"{total_amount:.2f}"]})
            monthly_summary = pd.concat([monthly_summary, total_row], ignore_index=True)
            monthly_summary['Amount'] = pd.to_numeric(monthly_summary['Amount'], errors='coerce')
            monthly_summary['Amount'] = monthly_summary['Amount'].map(lambda x: f"{x:.2f}")
            monthly_summary_filename = os.path.join(self.output_directory, f"{month}_monthly_summary.csv")
            monthly_summary.to_csv(monthly_summary_filename, index=False)
            print(f"Monthly summary for {month} has been saved to {monthly_summary_filename}")

    def save_weekly_summary(self, detailed_expenses):
        """
        Save weekly summary reports.
        """
        detailed_expenses['Week'] = detailed_expenses['Date'].dt.to_period('W').apply(lambda r: r.start_time)
        unique_weeks = detailed_expenses['Week'].unique()

        for week in unique_weeks:
            weekly_data = detailed_expenses[detailed_expenses['Week'] == week]
            weekly_summary = pd.pivot_table(
                weekly_data,
                values='Amount',
                index=['Week'],
                columns=['Category'],
                aggfunc='sum'
            ).reset_index().melt(
                id_vars=['Week'],
                var_name='Category',
                value_name='Amount'
            ).dropna()

            weekly_summary = weekly_summary[(weekly_summary['Category'] != 'Nothing') & (weekly_summary['Amount'] != 0)]
            weekly_summary['Amount'] = pd.to_numeric(weekly_summary['Amount'], errors='coerce')
            total_amount = weekly_summary['Amount'].sum()
            total_row = pd.DataFrame({'Week': [week], 'Category': ['Total'], 'Amount': [f"{total_amount:.2f}"]})
            weekly_summary = pd.concat([weekly_summary, total_row], ignore_index=True)
            weekly_summary['Amount'] = pd.to_numeric(weekly_summary['Amount'], errors='coerce')
            weekly_summary['Amount'] = weekly_summary['Amount'].map(lambda x: f"{x:.2f}")
            week_start_date = pd.to_datetime(week).strftime('%Y-%m-%d')
            weekly_summary_filename = os.path.join(self.output_directory, f"{week_start_date}_weekly_summary.csv")
            weekly_summary.to_csv(weekly_summary_filename, index=False)
            print(f"Weekly summary for {week_start_date} has been saved to {weekly_summary_filename}") 